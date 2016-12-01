# SMI Audit Lib

## Copyright

Copyright (c) 2016, Microsoft Corporation

All rights reserved. Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

## About

This feature contains a new library and header file that aid in the cataloging and verification of SMI handlers installed in a system.

The basic concept divides the SMI handler registration process into _handlers_ and _dispatchers_, which can be thought of as _sources_ and _sinks_. The _source_ is driver code that attempts to register a SMI handler through either the `PiSmmCore` dispatcher or another, custom dispatcher. The _sink_ is the dispatcher code that detects a registration and either records it for later dispatching, or perhaps wraps in with another function and registers it with a different dispatcher. By dividing the registration process like so, we can make sure that all registrations are detected and that all detections were notified prior.

## Handler Registration

The `SmmAuditLib.h` header file defines two macros for "announcing" a handler registration.

```c
#define SMI_REGISTER_NOTIFY(Handler)
#define SMI_REGISTER_NOTIFY_W_CONTEXT(Handler, Context)
```

The `SMI_REGISTER_NOTIFY()` function is called from the driver that is about to register a new handler, with the function pointer being registered as the sole argument, as seen here:

```c
SMI_REGISTER_NOTIFY( SmmVariableHandler );
Status = gSmst->SmiHandlerRegister (SmmVariableHandler, &gEfiSmmVariableProtocolGuid, &VariableHandle);
```

It should be noted that the registration interface may not always be `gSmst->SmiHandlerRegister()`. It is up to the developer to recognize when they are invoking an interface that would be responsible for registering a new handler.

The `SMI_REGISTER_NOTIFY_W_CONTEXT()` macro is provided as a convenience function. An example of its use can be seen in _MdeModulePkg\Core\PiSmmCore\PiSmmCore.c::SmmMain()_.

On their own, these macros will only output debug strings that help a developer locate the handler regsitration in the code. These strings have a newly defined debug level: `DEBUG_SMI` or `EFI_D_SMI`. However, if the `SmmAuditLibEnforceNotify` instance of the `SmmAuditLib` is used -- instead of `SmmAuditLibNull` -- an `ASSERT()` will occur if there is not a 1:1 relationship between handler registration requests and handler registration detections. See the _Audit Enforcement_ section below.

## Registration Detection

The `SmmAuditLib.h` header file defines another macro for "detecting" a handler registration.

```c
#define SMI_REGISTRATION_DETECT(Handler)
```

This macro is called from the driver that receives registration requests and maintains a list of handlers to be dispatched (otherwise known as the dispatcher). The sole parameter should be the function pointer being registered, as seen here:

```c
EFI_STATUS
EFIAPI
SmiHandlerRegister (
  IN  EFI_SMM_HANDLER_ENTRY_POINT2  Handler,
  IN  CONST EFI_GUID                *HandlerType  OPTIONAL,
  OUT EFI_HANDLE                    *DispatchHandle
  )
{
  SMI_HANDLER  *SmiHandler;
  SMI_ENTRY    *SmiEntry;
  LIST_ENTRY   *List;

  SMI_REGISTRATION_DETECT( Handler );

  ...
```

As with `SMI_REGISTER_NOTIFY()`, on its own this macro will only output debug strings (with the same `DEBUG_SMI` debug level). However, if the `SmmAuditLibEnforceNotify` instance is used, it will also work to maintain the 1:1 relationship between notifications and detections.

## Audit Enforcement

By default, the `SmmAuditLibNull` instance of the newly defined lib will do nothing other than output debug messages. However, if the `SmmAuditLibEnforceNotify` instance is used, all usages of `SMI_REGISTER_NOTIFY()` will record the address of the handler function being registered. The very next usage of `SMI_REGISTRATION_DETECT()` will then compare the detected pointer to the recorded pointer.

An `ASSERT()` will occur if:

* The recorded pointer does not match the detected pointer.
* A handler registration is detected, but there is no notification recorded.
* A notification is recorded, but a prior notification was never detected/registered.

The enforcement of these properties maintains a 1:1 relationship between handler registration requests and handler registration detections.
