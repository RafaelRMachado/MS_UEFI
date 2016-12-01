/** @file -- SmmAuditLibEnforceNotify.h
This file contains an implementation of the SmmAuditLib that will assert
if an SMI handler is registered that was not expected.
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

#ifndef _SMM_AUDIT_LIB_ENFORCE_NOTIFY_H_
#define _SMM_AUDIT_LIB_ENFORCE_NOTIFY_H_

//
// Table contents definition.
//
typedef struct _SMM_AUDIT_LIB_ENFORCE_NOTIFY_TABLE
{
  UINTN   NotifiedHandler;
} SMM_AUDIT_LIB_ENFORCE_NOTIFY_TABLE;

extern EFI_GUID     gEfiSmmAuditLibEnforceNotifyTableGuid;

#endif // _SMM_AUDIT_LIB_ENFORCE_NOTIFY_H_
