# Windows DWM Core Library Elevation of Privilege Vulnerability (CVE-2024-30051) (Published August 15 of 2024)

In this blog post, I will explain a vulnerability in the Microsoft
Windows DWM Core library that I analyzed when the exploit for Core
Impact was being developed. Allows an unprivileged attacker to execute
code as a DWM user with Integrity System privileges
([CVE-2024-30051](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2024-30051)).

Since there were not enough public information at the time to develop
the exploit, I had to reverse a lot, so here I will show how to reverse
the KB5037771 patch for Windows 23H2 using IDA PRO, I will use BINDIFF
to perform binary diffing between dwmcore.dll version 10.0.22621.3447
and version 10.0.22621.3593, will show how the heap overflow is
produced, and then will exploit it by elevating privileges, finally will
create a functional PoC.

Index:

[Windows DWM Core Library Elevation of Privilege Vulnerability
(CVE-2024-30051)
[1](#windows-dwm-core-library-elevation-of-privilege-vulnerability-cve-2024-30051)](#windows-dwm-core-library-elevation-of-privilege-vulnerability-cve-2024-30051)

[Vulnerability details:
[2](#vulnerability-details)](#vulnerability-details)

[Diffing to find the bug:
[3](#diffing-to-find-the-bug)](#diffing-to-find-the-bug)

[Analysis of the PoC exploiting CVE-2024-30051:
[8](#analysis-of-the-poc-exploiting-cve-2024-30051)](#analysis-of-the-poc-exploiting-cve-2024-30051)

[1)Initialization [8](#initialization)](#initialization)

[2)Hooking [8](#hooking)](#hooking)

[3)Creating the window [16](#creating-the-window)](#creating-the-window)

[4)Create Device [16](#create-device)](#create-device)

[5) Create Factory [22](#create-factory)](#create-factory)

[6) Create Device Context
[28](#create-a-device-context)](#create-a-device-context)

[7)Create Composition Device
[29](#create-a-composition-device)](#create-a-composition-device)

[8)Calling hook3 function
[31](#calling-dcompositioncreatedevice-function)](#calling-dcompositioncreatedevice-function)

[9)Creating target for HWND
[32](#creating-a-target-for-handle-hwnd)](#creating-a-target-for-handle-hwnd)

[10)Creating Surface [33](#creating-surface)](#creating-surface)

[11)Calling BeginDraw, EndDraw, and CreateVisual.
[34](#calling-begindraw-enddraw-and-createvisual)](#calling-begindraw-enddraw-and-createvisual)

[11)Calling Visual SetContent
[36](#calling-visual-setcontent)](#calling-visual-setcontent)

[12)Release objects [38](#release-objects)](#release-objects)

[13)Commit Composition Device
[38](#commit-composition-device)](#commit-composition-device)

[14)Calling hook2 [39](#calling-hook2)](#calling-hook2)

[15)Calling hook
[39](#remember-that-the-vulnerable-function-can-be-reached-using-some-methods-of-the-cprimitivegroup-class.-at-this-point-it-creates-a-heap-then-hook2-captures-and-saves-the-corresponding-heaphandle.)](#remember-that-the-vulnerable-function-can-be-reached-using-some-methods-of-the-cprimitivegroup-class.-at-this-point-it-creates-a-heap-then-hook2-captures-and-saves-the-corresponding-heaphandle.)

[16)Calling hook4
[41](#calling-the-function-hook4)](#calling-the-function-hook4)

[17)Performing Heap Spray
[49](#performing-heap-spray)](#performing-heap-spray)

[18)Modifying the base chunk previous to send.
[51](#modifying-the-base-chunk-before-send)](#modifying-the-base-chunk-before-send)

[19)Debugging the process DWM
[52](#debugging-the-dwm-process)](#debugging-the-dwm-process)

[20)Elevating Privileges to Integrity System level
[62](#elevating-privileges-to-integrity-system-level)](#elevating-privileges-to-integrity-system-level)

### Vulnerability details:

Windows DWM Core Library Elevation of Privilege Vulnerability
[CVE-2024-30051](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2024-30051)

Released: May 14, 2024

Assigning CNA: [Microsoft
CVE-2024-30051](https://msrc.microsoft.com/update-guide/en-US/advisory/CVE-2024-30051)

Impact: Elevation of Privilege

Max Severity: Important

Weakness:

> [CWE-122](https://cwe.mitre.org/data/definitions/122.html): Heap-based
> Buffer Overflow
>
> CVSS: 3.1 7.8 / 7.2

The vulnerability exists due to a size miscalculation error in an
integer division within the main Windows DWM library called
**dwmcore.dll**. A local user can cause a buffer overflow on the heap in
the **CCommandBuffer::Initialize** method in **dwmcore.dll** and can
execute arbitrary code with the **DWM** user with Integrity System
Privileges. The exploit will perform a Heap Spray in the **DWM** process
to prepare the memory and finally produces a Heap Overflow in
**dwmcore.dll** which will be triggered by releasing certain parts of
the heap spray.

Once the exploit is successful, the DWM process will load our crafted
DLL that executes our code or our executable (in our case a CMD) as the
DWM user which has Integrity System Privileges.

![](./media/image1.png)

Let’s walk through this vulnerability and see how it allows us to run as
a DWM user with Integrity Level SYSTEM. Note that since this is not a
user belonging to the Administrator group it has some privilege
restrictions.

### Diffing to Find the Bug: 

The patch for Windows 11 23H2 can be downloaded from:

<https://www.catalog.update.microsoft.com/Search.aspx?q=KB5037771>

windows11.0-kb5037771-x64_19a3f100fb8437d059d7ee2b879fe8e48a1bae42.msu

The vulnerable version of **dwmcore.dll** is: 10.0.22621.3447

The patched version of **dwmcore.dll** is: 10.0.22621.3593

Analyzing the changed functions, it’s clear the patched version of
**CCommandBuffer::Initialize** has a lot of blocks added, making it look
quite different to the unpatched version.

![](./media/image2.png)

After statically reversing that function, there are two calls to
**CD2DSharedBuffer::GetBufferSize.**

The first call gets the **size** to allocate in the **new** and the
second call gets the same size for the **memcpy**.

![](./media/image3.png)

Everything initially seems correct. However, before the allocation, it
performs some operations with the size.

![](./media/image4.png)

It gets **buffer_size** and **buffer_size2** by calling the same
**CD2DSharedBuffer::GetBufferSize** function, returning both the same
value. But in the **new** it performs a pre-operation, an integer
division of the **buffer_size** by 0x90 and then multiplying by 0x90,
whereas in the **memcpy** it uses the returned **buffer_size2** without
operating on it.

With these operations, I found that the size finally used in the **new**
and in the **memcpy** can be different.

**buffer_size = buffer_size2** (sizes returned)

**size_new= buffer_size/0x90 x 0x90**

**size_memcpy=buffer_size2**

For example, if **buffer_size** is **0x91**

**buffer_size = buffer_size2=0x91**

**size_new**= **buffer**\_**size**/0x90 x 0x90 **=0x90**

**size_memcpy**= **buffer_size2= 0x91**

This example proves that there is a **heap overflow**. It’s copying more
bytes than allocated, and the size is controllable.

For example, if buffer_size is **0x23f** as they used in the **POC.**

**buffer_size = buffer_size2=0x23F**

**size_new**= **buffer_size**/0x90 x 0x90 **=0x1b0**

**size_memcpy**== **buffer_size2=0x23f**

With the vulnerable function analyzed, I wanted to see how to reach the
vulnerable function **CCommandBuffer::Initialize**. This is where things
start to get complicated.

Looking back at the references to this function, it seems to be reached
from methods of the **CPrimitiveGroup** class:

![](./media/image5.png)

Such methods can be accessed from the **vftable** of **CPrimitiveGroup**
objects:![](./media/image6.png)

It has its **constructor:**

![](./media/image7.png)

And it is reached this way:

![](./media/image8.png)

As I went through this process initially, I took the time to read the
[PDF](https://conference.hitb.org/hitbsecconf2023ams/materials/D1T1%20-%20The%20Lost%20World%20of%20DirectComposition-%20Hunting%20Windows%20Desktop%20Window%20Manager%20Bugs%20-%20Zhang%20WangJunjie,%20YiSheng%20He%20&%20Wenyue%20Li.pdf)
“The Lost World of DirectComposition: Hunting Windows Desktop Window
Manager Bugs” and dove into the world of Direct Composition. This helped
me to create my first PoC.

Also, I needed to reverse **win32ksys** and tried to send packages
through the functions:

- **NtDCompositionCreateChannel**

- **NtDCompositionProcessChannelBatchBuffer**

- **NtDCompositionCommitChannel**

![](./media/image9.png)

My first PoC reached the **CPrimitiveGroup** constructor. However, after
a lot of reversing I did not find a way to handle the calls to the
**vftable** methods to get to the vulnerable function directly through
**ALPC** calls using these functions.

I spent a lot of time doing some complicated reversing. During this
process, I found the sample of the malware that exploited the
vulnerability, which was immensely helpful because the exploitation
method is much more complex than I initially thought. It also includes
several hookings to system APIs and uses methods that are perhaps a
little questionable. But anything is valid in war and exploits, so I
began to analyze the malware and from that analysis I created my final
PoC that finally exploits the vulnerability, which I will explain below.

First, I want to clarify that the malware not only exploits the
**CVE-2024-30051** vulnerability that elevates our process to Integrity
System Level, but it also performs a second part which from there ends
up elevating a SYSTEM user with all privileges, which already exceeds
the CVE explained.

Additionally, it’s important to note that the malware is much more
complex than my PoC that tries to minimize the code. The malware
performs many more checks to ensure reliability and because of that it
works on the first try. I discarded all those checks to simplify and
dedicated myself to pure exploitation, even perhaps having to run the
PoC two or three times to achieve the exploitation.

# 

# Analysis of the PoC exploiting CVE-2024-30051:

## 1)Initialization

The link to the executable PoC is
<https://github.com/fortra/CVE-2024-30051>

First, the PoC calls to **GetVersion** to get the OS version where it is
running and according to that it performs different initializations of
some global variables. My PoC was tested on Windows 11 23H2 and Windows
11 22h2. Other systems are vulnerable too and added the values to
exploit them.

## 2)Hooking 

It hooks four system functions and without hooking them it cannot
achieve the exploitation. These systems are: **RtlAllocateHeap,
RtlCreateHeap, NtDCompositionCreateChannel and
NtDCompositionCommitChannel**.

![](./media/image10.png)

In these functions it will patch the first 5 bytes to make it jump to
its own code. Of course, the code cannot be very far away since a 5-byte
jump does not cover all the memory and must be near.

To make it, the malware uses a very long code, analyzing the memory map
to decide where it can perform the allocation of its own code. As the
code is complicated, I focused on making it two simple lines:

**base_ntdll** = **GetModuleHandleW**(L"ntdll.dll");

global4\_ = (char \*)**VirtualAlloc**((LPVOID)(base_ntdll-0x2000),
0x1000uLL, 0x3000u, 0x40u);

I subtracted from the **ntdll base**, 0x2000 and I passed that address
to **VirtualAlloc** to allocate there.  
The 64-bit DLL are mapped quite separately in the memory from each other
with empty spaces between them.  
Let's see how the **hooks** work:

![](./media/image11.png)

It calls a **hooking** function, which is the one that will perform the
hooking of the **RtlAllocateHeap** API, which has three arguments, the
first is the address of the API to be patched, called
**sym_RtlAllocateHeap.**  
Before patching, it points to the start of the API:

![](./media/image12.png)

Here is the **RtlAllocateHeap** function:

![](./media/image13.png)

The second argument is the routine called **hook** that will be executed
when the API is completely patched:

![](./media/image14.png)

![](./media/image15.png)

The **hook** function calls **my_RtlAllocateHeap**.

The **hooking** function will patch the first 5 bytes of the api so that
it jumps to **hook**.

It will call the code in the allocated area where it will execute the
first API instruction that was stepped with the 5 bytes and then jump to
**RtlAllocateHeap+5** right after the patched bytes:

![](./media/image16.png)

This is how the API will look after the hook. The first 5 bytes changed
so that it jumps to **hook**. It will call **my_RtlAllocateHeap** the
code that is just above which will return to the area marked in purple
to continue the execution of the API:

![](./media/image17.png)

When the API finishes executing it will return to **hook.** From there
it will compare the global variable **heap_base** (which is initially
**zero)** with the first argument passed to **RtlAllocateHeap:**

![](./media/image18.png)

After that the code waits for a certain special allocation, which has a
specific **HeapHandle**. At the beginning this variable is zero and as
long as it is zero it will skip and work like a normal
**RtlAllocateheap:**

![](./media/image19.png)

The parameter **HeapHandle** is obtained inside **RtlCreateHeap** which
coincidentally is the second hooked API.

Looking for references to the global variable **heap_base**, it only
changes its value in the **hook2** function, which is the one that is
executed after hooking **RtlCreateHeap:**

![](./media/image20.png)

![](./media/image21.png)

So, the idea is to capture a certain **HeapHandle** and save it in
**heap_base**. Since it is now different from zero, the function
**hook** will start to compare each allocation. So, the PoC will save
the address memory who has the same **HeapHandle** as the previously
stored.

When this is the case, it will save the direction of the allocation to
the variable named **base:**![](./media/image22.png)

These first two hooks are now chained. When **hook2** saves the expected
value of **HeapHandle**, it activates the **hook** function that will
save the allocation address which uses the same **HeapHandle**.

The third hook is submitted to **NtDCompositionCreateChannel**. The
first time it is called it will save the **MappedAddress,** which is the
content of the third argument. From there it will change **hooked_flag**
to 1 so from then on it will not save anymore and will work normally.

![](./media/image23.png)

![](./media/image24.png)

The address saved in variable **base** will be read later three times.
Two of them will occur in the last hook, called **hook4:**

![](./media/image25.png)

The function **hook4** to **NtDCompositionCommitChannel** will be
analyzed later on because it's quite complex and very important.

## 3)Creating the Window

After the four hooks are completed, it returns to the main function to
start creating a window. This is done by calling **RegisterClassExW.**
However, to register a window class for later use, it should be called
with [CreateWindowExW
function.](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw)

![](./media/image26.png)

This initializes the COM library by calling **CoInitializeEx** to be
used by the calling thread:

![](./media/image27.png)

It calculates the required size of the window rectangle, based on the
desired size:

![](./media/image28.png)

The function **CreateWindowExW** is called to create a window that will
be drawn:

![](./media/image29.png)

## 4)Create Device

From there, call **D3D11CreateDevice** to create a device or DirectX
device that represents the display adapter:

![](./media/image30.png)

![](./media/image31.png)

In my PoC **ppDevice** is named **d3dDevice** and **ppInmediateContext**
is named **d3dContext:**

![](./media/image32.png)

The argument **flags** need to be set to 0x20:

![](./media/image33.png)

Then call **AddRef:**

![](./media/image34.png)

This increments the reference counter for an interface pointer to a COM
object:

![](./media/image35.png)

![](./media/image36.png)

The value 0x10 is subtracted to **THIS**:![](./media/image37.png)

![](./media/image38.png)

In the offset **0xf8** from **ID3D11Device-0x10** there is a pointer to
**TComObject:**

![](./media/image39.png)

![](./media/image40.png)

![](./media/image41.png)

This will be the new **THIS** and ends up jumping to
**TComObject::AddRef:**

![](./media/image42.png)

And it ends by adding one to the **object counter** that's in
**TComObject's** offset 8:

![](./media/image43.png)

Then, **AddRef** will increase the counter of the other object type
created in **D3D11CreateDevice,** which is type **ID3D11DeviceContext:**

![](./media/image44.png)

In this case, to find the new **THIS**, it subtracts 0x108:

![](./media/image45.png)

![](./media/image46.png)

It jumps here where in offset 0x98 is the new **THIS:**

![](./media/image47.png)

This is the counter. In this example, it's a **QWORD:**

![](./media/image48.png)

## 5) Create Factory

The PoC calls **D2D1CreateFactory** to use Direct2D, and to create the
**ID2D1Factory** interface that is used to create other Direct2D
resources that can be used to draw or describe shapes:

![](./media/image49.png)

The **riid** argument is the one suggested by the Microsoft page:

<https://learn.microsoft.com/en-us/windows/win32/api/d2d1/nf-d2d1-d2d1createfactory>

![](./media/image50.png)

These are the malware uses:

![](./media/image51.png)

The right one for **ID2D1Factory** can be found here**:**

<https://github.com/apitrace/dxsdk/blob/master/Include/d2d1_1.h>

![](./media/image52.png)

Since I am not an expert in **Direct Composition**, I then used the same
steps as the malware:

The new factory that returns does not provide any detailed type. It says
**void \*,** which means it is not officially documented:

![](./media/image53.png)

As I don't know an object type like in this case, I developed an
executable that uses it to see it in memory easily:

![](./media/image54.png)

Add breakpoints in the four **hook** functions. In this case, a
breakpoint in **hook2** will show when it captures the **HeapHandle:**

![](./media/image55.png)

The **hook** should be stopped when the desired **chunk** is captured:

![](./media/image56.png)

Put breakpoints in the other two **hooks:**

![](./media/image57.png)

Then it continues calling **QueryInterface:**

![](./media/image58.png)

<https://help.solidworks.com/2020/english/api/sldworksapi/queryinterface_example_cplusplus_com.htm>

<https://github.com/tpn/winsdk-10/blob/master/Include/10.0.16299.0/shared/dxgi.idl>

![](./media/image59.png)

It tries to perform a kind of dynamic casting. If the object of type
**ID3D11Device** can accept the interface (use the methods, etc.) of
**IDXGIDevice**, it creates a copy of the original object that accepts
new type, after that returns the pointer to it. In this case the
variable **d3dContext1** will be type **IDXGIDevice:**

![](./media/image60.png)

Both objects inherit from **CLayeredObject\<Cdevice\>**

The original **ID3D11Device** is**:**

![](./media/image61.png)

Like the one which returns the pointer.

![](./media/image62.png)

Then it creates an **ID2D1Device** object with the function
**CreateDevice**:

![](./media/image63.png) ![](./media/image64.png)

In **value2** it returns an object of type **ID2D1Device.**

## 

## 6) Create a Device Context

At this point, the PoC creates a new device context from a **Direct2d**
device. Using the function **CreateDeviceContext**

<https://learn.microsoft.com/en-us/windows/win32/api/d2d1_1/nf-d2d1_1-id2d1device-createdevicecontext>

![](./media/image65.png)

Here it is implemented in the PoC:

![](./media/image66.png)

![](./media/image67.png)

## 7)Create a Composition Device

Then call **DCompositionCreateDevice**

<https://learn.microsoft.com/en-us/windows/win32/api/dcomp/nf-dcomp-dcompositioncreatedevice>

![](./media/image68.png)

![](./media/image69.png)

The IID belongs to \_**IDCompositionDevice**

![](./media/image70.png)

![](./media/image71.png)

## 8)Calling DCompositionCreateDevice Function 

At the same moment that the function is traced over
**DCompositionCreateDevice**, it stops at **hook3**, when it calls
**NtDCompositionCreateChannel:**

![](./media/image72.png)

This way it's capturing the **MappedAddress** that the system uses
internally when **DCompositionCreateDevice** was called:

This is the Call stack up to here:

![](./media/image73.png)

This is the point where the **dcomp** module calls to function
**NtDCompositionCreateChannel:**

![](./media/image74.png)

## 9)Creating A Target for Handle HWND

After returning from previous step, save the **MappedAddress.** Using
**ALPC**, it will connect to the **DWM** process and then call
**CreateTargetForHwnd**

<https://learn.microsoft.com/en-us/windows/win32/api/dcomp/nf-dcomp-idcompositiondevice-createtargetforhwnd>

![](./media/image75.png)

It uses the handle **HWND** of the created window. It is related to the
device that I just created, which is the **THIS** of this method:

![](./media/image76.png)

## 10) Creating Surface

Then call **CreateSurface**

https://learn.microsoft.com/en-us/windows/win32/api/dcomp/nf-dcomp-idcompositiondevice-createsurface

![](./media/image77.png)

## 11) Calling BeginDraw, EndDraw, and CreateVisual

Then call **BeginDraw**, **EndDraw,** and arrive at **CreateVisual**.

![](./media/image78.png)

It calls **BeginDraw**

<https://learn.microsoft.com/en-us/windows/win32/api/dcomp/nf-dcomp-idcompositionsurface-begindraw>

This uses the IID **\_IDXGISurface:**

![](./media/image79.png)

![](./media/image80.png)

Then it uses **EndDraw:**

<https://learn.microsoft.com/en-us/windows/win32/api/dcomp/nf-dcomp-idcompositionsurface-enddraw>

![](./media/image81.png)

![](./media/image82.png)

Finally, it calls **CreateVisual:**

<https://learn.microsoft.com/en-us/windows/win32/api/dcomp/nf-dcomp-idcompositiondevice-createvisual>

![](./media/image83.png)

## 12)Calling Visual SetContent

Next, it calls **IDCompositionVisual::SetContent:**

<https://learn.microsoft.com/en-us/windows/win32/api/dcomp/nf-dcomp-idcompositionvisual-setcontent>

![](./media/image84.png)

![](./media/image85.png)

And it calls **SetRoot:**

![](./media/image86.png)

The **updateObject** that is received in **BeginDraw** does not specify
what type it is in the documentation.

![](./media/image87.png)

## 13)Release Objects

Next, it releases the previous created objects:

![](./media/image88.png)

## 14)Commit Composition Device

And now using the same **dcompDevice** object of type
**IDCompositionDevice,** it calls to the Commit method:

![](./media/image89.png)

![](./media/image90.png)

## 15)Calling hook2

Calling that **Commit** method stops on **hook2** that captures the
**desired HeapHandle:**

![](./media/image91.png)

This is the call stack now:

![](./media/image92.png)

## Remember that the vulnerable function can be reached using some methods of the **CPrimitiveGroup** class. At this point it creates a **Heap,** then **hook2** captures and saves the corresponding **HeapHandle**.

## 16)Calling the Function Hook

Before returning to the main, it also creates a chunk using
**RtlAllocateHeap**. It is then caught and stored in the base variable
inside the **hook** function:

![](./media/image93.png)

![](./media/image94.png)

The calls to **Create** and **Allocate** are performed one after the
other:

![](./media/image95.png)

Both (**Allocate** and **Create**) are called from
**DirectComposition::Cdevice::Commit:**

![](./media/image96.png)

## 17)Calling the Function hook4

After that, when **NtDCompositionCommitChannel** is called, it stops at
**hook4:**

![](./media/image97.png)

**NtDCompositionCommitChannel** is called from here:

![](./media/image98.png)

It is also called from **DirectComposition::Cdevice::Commit**

![](./media/image99.png)

It is worth mentioning that the system has already batched the commands
to send by **ALPC** to **DWM.** After that it sends commands using
**NtDCompositionCommitChannel.**  
The function **hook4** intercepts the **NtDCompositionCommitChannel**
calls and at this point more commands will be added to the batch.

Let's see what **hook4** does:

A loop is performed through the chunk pointed for **base**.

It exits the loop when finds the value **0x120** inside the chunk:

![](./media/image100.png)

It stores the address and the offset where **0x120** value was located:

![](./media/image101.png)

It overwrites the **0x120** value with **value4**, which is equal to
**0x1b0** + **0x8f =** **0x23f.** This is the size that it will use in
**memcpy** when it overflows:

![](./media/image102.png)

![](./media/image103.png)

![](./media/image104.png)

It adds **0xbc** + **0x90** to the address pointer where the **0x120**
was located:

![](./media/image105.png)

![](./media/image106.png)

Remember that at offset **0x48** from **base** was **0x120** size. That
was overwritten by **0x23f**, therefore the original chunk must be size
**0x120:**

The source is the pointer address of **0x23f + 0x2c:**

![](./media/image107.png)

It initially added **0x90** but it is now subtracting **0x90** again.  
The destination will be the address of the **pointer to 0x120** +
**0xbc:**

![](./media/image108.png)

It is going to write on this:

![](./media/image109.png)

All the writings will be inside the chunk:

![](./media/image110.png)

It is going to repeat the loop 3 times, which is the result of the
entire division of **0x1b0/0x90:**

![](./media/image111.png)

After that, as the **ArgChannelHandle** channel is the same one that was
used when the **MappedAddress** was captured. The PoC will add commands
to the **batch** using **NtDCompositionProcessChannelBatchBuffer.**
These will be processed along with those that the system had added**.**
The **batch** collects them and then the commands are sent all together
using **NtDCompositionCommitChannel:**

![](./media/image112.png)

![](./media/image113.png)

The command sent has the value 8, which corresponds to
**SetResourceIntegerProperty** for 4 different trackers (1,2,3, and 4).

## 18)Performing Heap Spray

When the PoC returns to the main function, it creates a different
channel to perform the HeapSpray.

It batches 0x10000 commands, which are sent with
\_**NtDCompositionCommitChannel:**

![](./media/image114.png)

This uses the value **CreateResource=1** and the type that corresponds
to **CHolographicInteropTextureMarshaler = 0x50:**

![](./media/image115.png)

The allocations are performed in the code below. The size of the objects
created to make the spray is **0x1b0:**

![](./media/image116.png)

It then performs a loop to release the objects created in the previous
step and now makes holes in memory distribution.  
The variable **counter2** begins at **0x3000** and adds steps of 0x20
while it is less than **0x7000**:

![](./media/image117.png)

## 19)Modifying the Base Chunk Before Send

It writes 0x41s from the direction of the chunk that was in **base** +
**0x48** + **44** + **0x1b0**  
That is, it is writing values that will be used later—when it overflows
the adjacent chunk:![](./media/image118.png)

That **pvalue7** is located on the address **0x224** from the **base**:

![](./media/image119.png)

Then it goes to the function “**escribe**”:

![](./media/image120.png)

It writes the **pKernelCallbacktable** plus **0x388,** the
**LoadLibraryA address** and the path to the DLL that will load. In this
case, I named it **s11.dll.**

![](./media/image121.png)

## 20)Debugging the DWM Process 

Now, a kernel debugger is needed to stop at the vulnerable function when
the heap overflow occurs. This is because the DWM process cannot be
debugged with a **user mode** debugger.

Using **IDA PRO** to remotely debug the target, set a conditional
breakpoint so it stops when the size is equal to **0x1b0:**

> print ("VALUE1 %x" % ((cpu.rax)))  
> return cpu.rax==0x1b0.

Since a user mode program is being debugged from kernel, a switch needs
to be made to the **DWM** process context to put the breakpoint. Reload
the user symbols with:

> **. reload /user**

Reload the kernel ones with:

> **. reload /f**

![](./media/image122.png)

It will stop when **ShowWindow** is stepped over**:**

![](./media/image123.png)

It allocates with size **0x1b0** and copies with size **0x23f,**
producing the heap overflow:

![](./media/image124.png)

At this point the call stack looks like this:![](./media/image125.png)

To create the overflow, the DWM receive values in the below code:

The crafted values in **base** sent from my PoC are read using
**MapViewofFile** from the **DWM process** in the module
**dwmcore.dll:**

![](./media/image126.png)

The previous function is called from:

![](./media/image127.png)

![](./media/image128.png)

When it is sent using **ALPC** from **hook4** using **destination_copy
(NtDCompositionCommitChannel)** it stops:

![](./media/image129.png)

Remember that in **hook4** commands, commands were added to the batch.
However, the system also had already added some commands to the batch,
including the **base** and the crafted data:

![](./media/image129.png)

![](./media/image130.png)

![](./media/image131.png)

In this case it shares a memory area that starts at
**000001cd'178d0000.** When it is used it as a source to perform the
**memcpy** it will be **0x794** bytes later in that same memory area.

The size of the shared memory area is **0x4000:**

![](./media/image132.png)

It will stop when the size to be allocated is **0x1b0,** and reaches the
**memcpy** to copy **0x23f** bytes:

![](./media/image133.png)

Beyond **0x1b0** in the memory is the code that will overflow
overwriting the adjacent block:![](./media/image134.png)

When the chunks are released from the PoC, it ends by jumping to
**LoadLibraryA** , which loads the crafted library:

![](./media/image135.png)

That comes from here:

![](./media/image136.png)![](./media/image137.png)

The Heap spray was made with **0x1b0** size objects of type
**CHolographicInteropTexture.**

Since I had made holes in the memory distribution, this releases some
objects. As the block that is going to overflow has as size **0x1b0**
too, it has a high probability of being located in the holes in the heap
spray.

At the destination of the **memcpy** the blocks are located every 0x1b0
bytes:

![](./media/image138.png)

The pointer to a **vftable** is overwritten by the pointer to
**LoadLibrary:**

Before overwriting:

![](./media/image139.png)

After overwriting:

![](./media/image140.png)

Remember that it ended jumping to **\[R11+50\],** which is the pointer
to **LoadLibraryA**.

## 21)Elevating Privileges to Integrity System Level

Executing the PoC, copy the DLL in the same path that figures in the
PoC:![](./media/image141.png)

After executing the PoC a **CMD** process is executed with **DWM** user
Integrity System Level privileges:

![](./media/image142.png)

References:

PoC at Fortra GitHub: <https://github.com/fortra/CVE-2024-30051>

<https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2024-30051>

<https://msrc.microsoft.com/update-guide/en-US/advisory/CVE-2024-30051>

This completes the PoC. Remember that if you execute it many times the
heap will remain in an unstable state, so you may need to restart the
machine to make it work again. Also, while it may not always work in the
first shot, it will typically work correctly during a second or third
attempt. As you can see, reversing can be difficult, so if you have any
questions, you can consult me.

Mail: <Ricardo.Narvaja@fortra.com>

X: @ricnar456
