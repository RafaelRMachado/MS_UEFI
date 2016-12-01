/** @file -- SmmAuditLib.h
This library contains defintions and helper functions to identify and audit
SMM handler registrations.

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

#ifndef _SMM_AUDIT_LIB_H_
#define _SMM_AUDIT_LIB_H_

/**
  This function will notify any dispatchers that this handler registration is
  approved and audited.

  @param[in]  Handler  This is a UINTN of the function pointer for the
                       handler being registered.

**/
VOID
EFIAPI
SmmAuditLibHandlerNotify (
  IN  UINTN    Handler
  );


/**
  This function will ensure that all handlers that are registered were
  notified first.

  This way, debug mode can easily catch when the code has been modified so that
  additional handlers are registered.

  @param[in]  Handler  This is a UINTN of the function pointer for the
                       handler being registered.

**/
VOID
EFIAPI
SmmAuditLibHandlerRegister (
  IN  UINTN    Handler
  );


//=========================================================
// SMI SOURCE MACROS
//  These macros should be used before a new SMI handler is registered
//  so that additional data about the handler can be located in the log.
//=========================================================

#define SMI_REGISTER_NOTIFY(Handler)                                                                            \
  do {                                                                                                          \
    DEBUG(( DEBUG_SMI, "<SMI-AUDIT> SMI-REGISTER-NOTIFY - HANDLER: %a() [0x%X]\n", #Handler, (UINTN)Handler )); \
    DEBUG(( DEBUG_SMI, "<SMI-AUDIT>   REGISTERED IN FILE: %a\n", __FILE__ ));                                   \
    DEBUG(( DEBUG_SMI, "<SMI-AUDIT>   FUNCTION::LINE: "__FUNCTION__"()::%d\n", __LINE__ ));                     \
    SmmAuditLibHandlerNotify( (UINTN)Handler );                                                                 \
  } while (FALSE)


#define SMI_REGISTER_NOTIFY_W_CONTEXT(Handler, Context)                                                                               \
  do {                                                                                                                                \
    DEBUG(( DEBUG_SMI, "<SMI-AUDIT> SMI-REGISTER-NOTIFY - HANDLER: %a() [0x%X], Context: %d\n", #Handler, (UINTN)Handler, Context )); \
    DEBUG(( DEBUG_SMI, "<SMI-AUDIT>   REGISTERED IN FILE: %a\n", __FILE__ ));                                                         \
    DEBUG(( DEBUG_SMI, "<SMI-AUDIT>   FUNCTION::LINE: "__FUNCTION__"()::%d\n", __LINE__ ));                                           \
    SmmAuditLibHandlerNotify( (UINTN)Handler );                                                                                       \
  } while (FALSE)


//=========================================================
// SMI REGISTRATION MACROS
//  These macros should be used any time a new handler is registered
//  with a dispatcher. Judgement can be used about how granular the
//  registration should be, so long as there is a 1:1 correlation
//  between SOURCEs and REGISTRATIONs.
//=========================================================

#define SMI_REGISTRATION_DETECT(Handler)                                                                        \
  do {                                                                                                          \
    DEBUG(( DEBUG_SMI, "<SMI-AUDIT> SMI-REGISTRATION-DETECT - HANDLER: [0x%X]\n", (UINTN)Handler ));            \
    DEBUG(( DEBUG_SMI, "<SMI-AUDIT>   DETECTED BY FUNCTION: "__FUNCTION__"()\n" ));                             \
    SmmAuditLibHandlerRegister( (UINTN)Handler );                                                               \
  } while (FALSE)

#endif // _SMM_AUDIT_LIB_H_
