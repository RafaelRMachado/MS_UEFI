/** @file -- SmmAuditLibNull.c
This file contains an empty, sample implementation of the SmmAuditLib.
This library is intended to aid in the identification and accounting of all SMI
handlers in a system.

Copyright (c) 2016, Microsoft Corporation

All rights reserved.
Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

**/

#include <Library/SmmAuditLib.h>


/**
  This function will notify any dispatchers that this handler registration is
  approved and audited.

  NOTE: Sample implementation does nothing.

  @param[in]  Handler  This is a UINTN of the function pointer for the
                       handler being registered.

**/
VOID
EFIAPI
SmmAuditLibHandlerNotify (
  IN  UINTN    Handler
  )
{
  return;
}


/**
  This function will ensure that all handlers that are registered were
  notified first.

  This way, debug mode can easily catch when the code has been modified so that
  additional handlers are registered.

  NOTE: Sample implementation does nothing.

  @param[in]  Handler  This is a UINTN of the function pointer for the
                       handler being registered.

**/
VOID
EFIAPI
SmmAuditLibHandlerRegister (
  IN  UINTN    Handler
  )
{
  return;
}
