/** @file -- SmmAuditLibEnforceNotify.c
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

#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SmmAuditLib.h>
#include <Library/SmmServicesTableLib.h>

#include "SmmAuditLibEnforceNotify.h"

SMM_AUDIT_LIB_ENFORCE_NOTIFY_TABLE  *mNotifyTable = NULL;

/**
  This funciton will check for an existing table in the Smst.
  If not found, it will attempt to create one.

  @retval     Pointer   Table was successfully found or created and registered.
  @retval     NULL      An error occurred.

**/
STATIC
SMM_AUDIT_LIB_ENFORCE_NOTIFY_TABLE*
FindOrCreateTable (
  VOID
  )
{
  EFI_STATUS                            Status = EFI_SUCCESS;
  UINTN                                 Index;
  SMM_AUDIT_LIB_ENFORCE_NOTIFY_TABLE    *NewTable = NULL;

  // Check the environment.
  if (gSmst == NULL)
  {
    DEBUG(( DEBUG_ERROR, __FUNCTION__" - Cannot find gSmst!\n" ));
    ASSERT( gSmst != NULL );
    return NULL;
  }

  //
  // Determine whether the table already exists.
  //
  for (Index = 0; Index < gSmst->NumberOfTableEntries; Index++)
  {
    if (CompareGuid( &(gSmst->SmmConfigurationTable[Index].VendorGuid), &gEfiSmmAuditLibEnforceNotifyTableGuid ))
    {
      NewTable = gSmst->SmmConfigurationTable[Index].VendorTable;
      break;
    }
  }

  //
  // If Index == NumberOfTableEntries, that means we walked the entire list without
  // finding the table. Ergo, we must create it.
  //
  if (Index == gSmst->NumberOfTableEntries)
  {
    // Create the table.
    Status = gSmst->SmmAllocatePool( EfiRuntimeServicesData, sizeof( *NewTable ), (VOID**)&NewTable );

    // If we're still rolling... init and register the table.
    if (!EFI_ERROR( Status ))
    {
      ZeroMem( NewTable, sizeof( *NewTable ) );
      Status = gSmst->SmmInstallConfigurationTable( gSmst,
                                                    &gEfiSmmAuditLibEnforceNotifyTableGuid,
                                                    NewTable,
                                                    sizeof( *NewTable ) );
    }

    // Make sure to clean up if there was a problem.
    if (EFI_ERROR( Status ) && NewTable != NULL)
    {
      gSmst->SmmFreePool( NewTable );
      NewTable = NULL;
    }
  }

  return NewTable;
} // InitNotifyTable()


/**
  This function will ensure that all handlers that are registered were
  notified first. Will ASSERT otherwise.

  This way, debug mode can easily catch when the code has been modified so that
  additional handlers are registered.

  NOTE: At some point, it may be interesting to attest to which handlers
        were registered so that the OS may take a different action if an
        un-audited handler is registered.

  @param[in]  Handler  This is a UINTN of the function pointer for the
                       handler being registered.

**/
VOID
EFIAPI
SmmAuditLibHandlerRegister (
  IN  UINTN    Handler
  )
{
  //
  // If the table hasn't been found yet, find or create it.
  //
  if (mNotifyTable == NULL)
  {
    mNotifyTable = FindOrCreateTable();

    // If the table STILL hasn't been found, we've got problems.
    if (mNotifyTable == NULL)
    {
      DEBUG(( DEBUG_ERROR, __FUNCTION__" - Cannot locate or create the EfiSmmAuditLibEnforceNotifyTableGuid!\n" ));
      ASSERT( mNotifyTable != NULL );
      return;
    }
  }

  //
  // Make sure that we were notified of this registration.
  //
  if (mNotifyTable->NotifiedHandler == 0 || mNotifyTable->NotifiedHandler != Handler)
  {
    DEBUG(( DEBUG_WARN, __FUNCTION__" - Unknown SMI handler registration attempted!\n" ));
    ASSERT( mNotifyTable->NotifiedHandler == Handler );
    return;
  }

  //
  // If we've made it this far, clear the notification.
  //
  mNotifyTable->NotifiedHandler = 0;

  return;
} // SmmAuditLibHandlerRegister()


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
  )
{
  //
  // If the table hasn't been found yet, find or create it.
  //
  if (mNotifyTable == NULL)
  {
    mNotifyTable = FindOrCreateTable();

    // If the table STILL hasn't been found, we've got problems.
    if (mNotifyTable == NULL)
    {
      DEBUG(( DEBUG_ERROR, __FUNCTION__" - Cannot locate or create the EfiSmmAuditLibEnforceNotifyTableGuid!\n" ));
      ASSERT( mNotifyTable != NULL );
      return;
    }
  }

  //
  // We've got the table, make sure that the coast is clear.
  //
  if (mNotifyTable->NotifiedHandler != 0)
  {
    DEBUG(( DEBUG_ERROR, __FUNCTION__" - SMI handler registration already pending!\n" ));
    ASSERT( mNotifyTable->NotifiedHandler == 0 );
    return;
  }

  //
  // If the coast is clear, register our handler.
  //
  mNotifyTable->NotifiedHandler = Handler;

  return;
} // SmmAuditLibHandlerNotify()
