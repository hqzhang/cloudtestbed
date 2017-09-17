/*_############################################################################
  _## 
  _##  v3_mib.cpp  
  _## 
  _##
  _##  AGENT++ API Version 3.5.22c
  _##  -----------------------------------------------
  _##  Copyright (C) 2000-2004 Frank Fock, Jochen Katz
  _##  
  _##  LICENSE AGREEMENT
  _##
  _##  WHEREAS,  Frank  Fock  and  Jochen  Katz  are  the  owners of valuable
  _##  intellectual  property rights relating to  the AGENT++ API and wish to
  _##  license AGENT++ subject to the  terms and conditions set forth  below;
  _##  and
  _##
  _##  WHEREAS, you ("Licensee") acknowledge  that Frank Fock and Jochen Katz
  _##  have the right  to grant licenses  to the intellectual property rights
  _##  relating to  AGENT++, and that you desire  to obtain a license  to use
  _##  AGENT++ subject to the terms and conditions set forth below;
  _##
  _##  Frank  Fock    and Jochen   Katz   grants  Licensee  a  non-exclusive,
  _##  non-transferable, royalty-free  license  to use   AGENT++ and  related
  _##  materials without  charge provided the Licensee  adheres to all of the
  _##  terms and conditions of this Agreement.
  _##
  _##  By downloading, using, or  copying  AGENT++  or any  portion  thereof,
  _##  Licensee  agrees to abide  by  the intellectual property  laws and all
  _##  other   applicable laws  of  Germany,  and  to all of   the  terms and
  _##  conditions  of this Agreement, and agrees  to take all necessary steps
  _##  to  ensure that the  terms and  conditions of  this Agreement are  not
  _##  violated  by any person  or entity under the  Licensee's control or in
  _##  the Licensee's service.
  _##
  _##  Licensee shall maintain  the  copyright and trademark  notices  on the
  _##  materials  within or otherwise  related   to AGENT++, and  not  alter,
  _##  erase, deface or overprint any such notice.
  _##
  _##  Except  as specifically   provided in  this  Agreement,   Licensee  is
  _##  expressly   prohibited  from  copying,   merging,  selling,   leasing,
  _##  assigning,  or  transferring  in  any manner,  AGENT++ or  any portion
  _##  thereof.
  _##
  _##  Licensee may copy materials   within or otherwise related   to AGENT++
  _##  that bear the author's copyright only  as required for backup purposes
  _##  or for use solely by the Licensee.
  _##
  _##  Licensee may  not distribute  in any  form  of electronic  or  printed
  _##  communication the  materials  within or  otherwise  related to AGENT++
  _##  that  bear the author's  copyright, including  but  not limited to the
  _##  source   code, documentation,  help  files, examples,  and benchmarks,
  _##  without prior written consent from the authors.  Send any requests for
  _##  limited distribution rights to fock@agentpp.com.
  _##
  _##  Licensee  hereby  grants  a  royalty-free  license  to  any  and   all 
  _##  derivatives  based  upon this software  code base,  that  may  be used
  _##  as a SNMP  agent development  environment or a  SNMP agent development 
  _##  tool.
  _##
  _##  Licensee may  modify  the sources  of AGENT++ for  the Licensee's  own
  _##  purposes.  Thus, Licensee  may  not  distribute  modified  sources  of
  _##  AGENT++ without prior written consent from the authors. 
  _##
  _##  The Licensee may distribute  binaries derived from or contained within
  _##  AGENT++ provided that:
  _##
  _##  1) The Binaries are  not integrated,  bundled,  combined, or otherwise
  _##     associated with a SNMP agent development environment or  SNMP agent
  _##     development tool; and
  _##
  _##  2) The Binaries are not a documented part of any distribution material. 
  _##
  _##
  _##  THIS  SOFTWARE  IS  PROVIDED ``AS  IS''  AND  ANY  EXPRESS OR  IMPLIED
  _##  WARRANTIES, INCLUDING, BUT NOT LIMITED  TO, THE IMPLIED WARRANTIES  OF
  _##  MERCHANTABILITY AND FITNESS FOR  A PARTICULAR PURPOSE  ARE DISCLAIMED.
  _##  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
  _##  INDIRECT,   INCIDENTAL,  SPECIAL, EXEMPLARY,  OR CONSEQUENTIAL DAMAGES
  _##  (INCLUDING,  BUT NOT LIMITED  TO,  PROCUREMENT OF SUBSTITUTE  GOODS OR
  _##  SERVICES; LOSS OF  USE,  DATA, OR PROFITS; OR  BUSINESS  INTERRUPTION)
  _##  HOWEVER CAUSED  AND ON ANY THEORY  OF  LIABILITY, WHETHER IN CONTRACT,
  _##  STRICT LIABILITY, OR TORT  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
  _##  IN  ANY WAY OUT OF  THE USE OF THIS  SOFTWARE,  EVEN IF ADVISED OF THE
  _##  POSSIBILITY OF SUCH DAMAGE. 
  _##
  _##
  _##  Stuttgart, Germany, Thu Jan 13 00:12:31 CET 2005 
  _##  
  _##########################################################################*/

#include <agent_pp/agent++.h>

#ifdef _SNMPv3

#ifndef WIN32
#include <unistd.h>
#endif

#include <agent_pp/tools.h> 
#include <agent_pp/v3_mib.h>
#include <snmp_pp/v3.h>
#include <snmp_pp/auth_priv.h>
#include <agent_pp/vacm.h>
#include <agent_pp/log.h>
#include <agent_pp/snmp_textual_conventions.h>

#ifdef SNMP_PP_NAMESPACE
using namespace Snmp_pp;
#endif

#ifdef AGENTPP_NAMESPACE
namespace Agentpp {
#endif


const index_info       	iUsmUserTable[2] = 
{{ sNMP_SYNTAX_OCTETS, FALSE, 5, 32  },
 { sNMP_SYNTAX_OCTETS, FALSE, 1, 32 }};
const unsigned int	lUsmUserTable = 2;

/**********************************************************************
 *  
 *  class V3SnmpEngineID
 * 
 **********************************************************************/
V3SnmpEngineID::V3SnmpEngineID(const v3MP *mp)
  : MibLeaf( oidV3SnmpEngineID, READONLY, new OctetStr("")), v3mp(mp) {}

void V3SnmpEngineID::get_request(Request* req, int index)
{
  *((OctetStr*)value) = v3mp->get_local_engine_id();
  MibLeaf::get_request(req, index);
}

/**********************************************************************
 *  
 *  class V3SnmpEngineBoots
 * 
 **********************************************************************/

V3SnmpEngineBoots::V3SnmpEngineBoots(const USM *u)
  : MibLeaf( oidV3SnmpEngineBoots, READONLY, new SnmpInt32(0)), usm(u) {}

void V3SnmpEngineBoots::get_request(Request* req, int index)
{
  long time, boots;
  usm->get_local_time(&boots, &time);
  *((SnmpInt32*)value) = boots;
  MibLeaf::get_request(req, index);
}

/**********************************************************************
 *  
 *  class V3SnmpEngineTime
 * 
 **********************************************************************/
V3SnmpEngineTime::V3SnmpEngineTime(const USM *u)
  : MibLeaf( oidV3SnmpEngineTime, READONLY, new SnmpInt32(0)), usm(u) {}

void V3SnmpEngineTime::get_request(Request* req, int index)
{
  long time, boots;
  usm->get_local_time(&boots, &time);
  *((SnmpInt32*)value) = time;
  MibLeaf::get_request(req, index);
}

/**********************************************************************
 *  
 *  class V3SnmpEngineMaxMessageSize
 * 
 **********************************************************************/
V3SnmpEngineMaxMessageSize::V3SnmpEngineMaxMessageSize():
  MibLeaf( oidV3SnmpEngineMaxMessageSize, READONLY, new SnmpInt32(V3MAXMESSAGESIZE))
{
}

/**********************************************************************
 *  
 *  class V3SnmpEngine
 * 
 **********************************************************************/

V3SnmpEngine::V3SnmpEngine(void): MibGroup(oidV3SnmpEngine) 
{
        v3MP *v3mp = v3MP::I;
	USM *usm = 0;
	if (v3mp)
	  usm = v3mp->get_usm();
        if ((!v3mp) || (!usm))
        {
	  LOG_BEGIN(ERROR_LOG | 0);
	  LOG("V3SnmpEngine: v3MP must be initialized before this MibGroup");
	  LOG_END;
        }

	add(new V3SnmpEngineID(v3mp));
	add(new V3SnmpEngineBoots(usm));
	add(new V3SnmpEngineTime(usm));
	add(new V3SnmpEngineMaxMessageSize());
}


UsmUserTableStatus::UsmUserTableStatus(const Oidx& o, int _base_len, USM *u)
  : snmpRowStatus(o, READCREATE), usm(u)
{
  base_len = _base_len;
}

UsmUserTableStatus::~UsmUserTableStatus()
{

}


/**
 * Set the receiver's value and backup its old value for a later undo. 
 *
 * @param vb - The variable binding that holds the new value.
 * @return SNMP_ERROR_SUCCESS if the new value has been set,
 *         SNMP_ERROR_WRONG_TYPE or SNMP_ERROR_BAD_VALUE otherwise.
 */

int UsmUserTableStatus::set(const Vbx& vb)
{
  undo = value->clone();
  uint32 rs;
  vb.get_value(rs);
  switch (rs) {
    case rowNotInService: {
      set_value(rowNotInService);
      my_table->row_deactivated(my_row, my_row->get_index());
      deleteUsmUser();
      break;
    }
    case rowActive:
    case rowCreateAndGo:
      set_value(rowActive);
      my_table->row_activated(my_row, my_row->get_index());
      addUsmUser();
      break;
    case rowCreateAndWait:
      set_value(rowNotReady);
      break;
    default: {
        deleteUsmUser();
        set_value(rs);
      }
  }
  return SNMP_ERROR_SUCCESS;
}

/** 
 * Undo a previous set.
 *
 * @return SNMP_ERROR_SUCCESS on success and SNMP_ERROR_UNDO_FAIL on failure.
 */

int UsmUserTableStatus::unset()
{
  uint32 rs;
  rs = *(SnmpInt32*)undo;

  switch (rs) {
    case rowEmpty:
      if ((get() == rowActive) || (get() == rowCreateAndGo))
        deleteUsmUser();
      if (value) delete value;
      value = undo;
      undo = 0;
      break;
    case rowActive:
      addUsmUser();
      if (value) delete value;
      value = undo;
      undo = 0;
      break;
    case rowNotInService:
    case rowNotReady:
      if (get() == rowActive)
        deleteUsmUser();
      if (value) delete value;
      value = undo;
      undo = 0;
      break;
    case rowCreateAndGo: {
      set_value(rowNotReady);
      Vbx vbx;
      vbx.set_value(rowActive);
      if (check_state_change(vbx))
        addUsmUser();
      else
        deleteUsmUser();
      break;
    }
    case rowCreateAndWait:
      if (get() == rowActive)
        deleteUsmUser();
      set_value(rowNotReady);
      break;
    default:
      if (value) delete value;
      value = undo;
      undo = 0;
  }
  return SNMP_ERROR_SUCCESS;
}

void UsmUserTableStatus::deleteUsmUser()
{
  OctetStr engineID, userName;
  my_row->get_nth(0)->get_value().get_value(engineID);
  my_row->get_nth(1)->get_value().get_value(userName);
  usm->delete_localized_user(engineID, userName);
}

void UsmUserTableStatus::addUsmUser()
{
  OctetStr engineID, userName, authKey, privKey;
  Oidx authOid, privOid;
  int authProt, privProt;
  my_row->get_nth(0)->get_value().get_value(engineID);
  my_row->get_nth(1)->get_value().get_value(userName);
  my_row->get_nth(4)->get_value().get_value(authOid);
  my_row->get_nth(5)->get_value().get_value(authKey);
  my_row->get_nth(7)->get_value().get_value(privOid);
  my_row->get_nth(8)->get_value().get_value(privKey);

  if ((UsmUserTable::auth_base.len() + 1 == authOid.len()) &&
      (UsmUserTable::auth_base.is_root_of(authOid)))
    authProt = authOid.last();
  else
    authProt = SNMPv3_usmNoAuthProtocol;
  
  if ((UsmUserTable::priv_base.len() + 1 == privOid.len()) &&
      (UsmUserTable::priv_base.is_root_of(privOid)))
    privProt = privOid.last();
  else
    privProt = SNMPv3_usmNoPrivProtocol;

  usm->add_localized_user(engineID, userName, userName,
			  authProt, authKey, privProt, privKey);
}



MibEntryPtr UsmUserTableStatus::clone()
{
  snmpRowStatus* other = new UsmUserTableStatus(oid, base_len, usm);
  other->set_reference_to_table(my_table);
  return other;
}


UsmUserTable *usm_user_table_ptr = NULL;

void usm_callback(const OctetStr &engine_id,
                  const OctetStr &usm_user_name,
                  const OctetStr &usm_user_security_name,
                  const int auth_protocol,
                  const OctetStr &auth_key,
                  const int priv_protocol,
                  const OctetStr &priv_key)
{

  LOG_BEGIN(DEBUG_LOG | 13);
  LOG("Callback called from USM (engine_id) (user_name)");
  LOG(engine_id.get_printable());
  LOG(usm_user_name.get_printable());
  LOG_END;

  if (usm_user_table_ptr)
    usm_user_table_ptr->addNewRow(engine_id,
                                  usm_user_name,
                                  usm_user_security_name,
                                  auth_protocol,
                                  auth_key,
                                  priv_protocol,
                                  priv_key,
                                  FALSE);
}

const Oidx UsmUserTable::auth_base = oidUsmAuthProtocolBase;
const Oidx UsmUserTable::priv_base = oidUsmPrivProtocolBase;

UsmUserTable::UsmUserTable(): StorageTable(oidUsmUserEntry, iUsmUserTable, 
					   lUsmUserTable)
{
  v3MP *v3mp = v3MP::I;
  if (!v3mp)
  {
    LOG_BEGIN(ERROR_LOG | 0);
    LOG("MPDGroup: v3MP must be initialized before the UsmUserTable");
    LOG_END;
  }

  usm = v3mp->get_usm();

  Oidx tmpoid = Oidx(oidUsmUserEntry);

  // usmUserEngineID
  add_col(new SnmpAdminString("1", NOACCESS, new OctetStr(""), 
			      VMODE_DEFAULT, 0, 32));
  // usmUserName
  add_col(new SnmpAdminString("2", NOACCESS, new OctetStr(""), 
			      VMODE_DEFAULT, 1, 32));
  // usmUserSecurityName
  add_col(new SnmpAdminString("3", READONLY, new OctetStr(""),
			      VMODE_DEFAULT, 0, 255));
  // usmUserCloneFrom
  add_col(new UsmCloneFrom("4"));
  // usmUserAuthProtocol
  add_col(new usmUserAuthProtocol("5", usm));
  // usmUserAuthKeyChange
  add_col(new UsmKeyChange("6", usm));
  // usmUserOwnAuthKeyChange
  add_col(new UsmOwnKeyChange("7", usm));
  // usmUserPrivProtocol
  add_col(new usmUserPrivProtocol("8", usm));
  // usmUserPrivKeyChange
  add_col(new UsmKeyChange("9", usm));
  // usmUserOwnPrivKeyChange
  add_col(new UsmOwnKeyChange("10", usm));
  // usmUserPublic
  add_col(new SnmpAdminString("11", READCREATE, new OctetStr(""),
			      VMODE_DEFAULT, 0, 32));
  // usmUserStorageType
  add_storage_col(new StorageType("12", 3));
  // usmUserStatus
  add_col(new UsmUserTableStatus("13", tmpoid.len(), usm));

  //initialize UsmKeyChange- and UsmOwnKeyChange-objects
  struct UsmUserTableEntry *user;
  int users = usm->get_user_count();
  for (int i = 1; i <= users; i++) {
    Oidx o;
    user = usm->get_user(i);
    
    o = Oidx::from_string(OctetStr(user->usmUserEngineID,
				   user->usmUserEngineIDLength), TRUE);
    o += Oidx::from_string(OctetStr(user->usmUserName,
                                    user->usmUserNameLength), TRUE);

    if (find_index(o))
      continue;

    MibTableRow *newRow = add_row(o);
    
    newRow->get_nth(2)->replace_value(new OctetStr((char*)(user->usmUserSecurityName)));
    newRow->get_nth(3)->replace_value(o.clone());

    UsmKeyChange *ukc5 = (UsmKeyChange*)newRow->get_nth(5);
    UsmKeyChange *ukc6 = (UsmKeyChange*)newRow->get_nth(6);
    UsmKeyChange *ukc8 = (UsmKeyChange*)newRow->get_nth(8);
    UsmKeyChange *ukc9 = (UsmKeyChange*)newRow->get_nth(9);

    if (user->usmUserAuthProtocol == SNMPv3_usmNoAuthProtocol)
    {
      newRow->get_nth(4)->replace_value(new Oid(oidUsmNoAuthProtocol));
      ukc5->initialize(0, 0, AUTHKEY, ukc6);
      ukc6->initialize(0, 0, AUTHKEY, ukc5);
      ukc8->initialize(0, 0, PRIVKEY, ukc9);
      ukc9->initialize(0, 0, PRIVKEY, ukc8);
    }
    else
    {
      Oid *authoid = new Oid(auth_base);
      *authoid += user->usmUserAuthProtocol;
      newRow->get_nth(4)->replace_value(authoid);

      Auth *auth = usm->get_auth_priv()->get_auth(user->usmUserAuthProtocol);
      int hashlength = 0;
      if (auth)
	hashlength = auth->get_hash_len();
      else
      {
	LOG_BEGIN(ERROR_LOG | 1);
	LOG("BUG: USM has user with unknown auth protocol");
	LOG(user->usmUserAuthProtocol);
	LOG_END;
      }

      ukc5->initialize(hashlength, user->usmUserAuthProtocol, AUTHKEY, ukc6);
      ukc6->initialize(hashlength, user->usmUserAuthProtocol, AUTHKEY, ukc5);
      ukc8->initialize(hashlength, user->usmUserAuthProtocol, PRIVKEY, ukc9);
      ukc9->initialize(hashlength, user->usmUserAuthProtocol, PRIVKEY, ukc8);
    }

    newRow->get_nth(5)->replace_value(new OctetStr(user->usmUserAuthKey,
						  user->usmUserAuthKeyLength));
    newRow->get_nth(6)->replace_value(new OctetStr(user->usmUserAuthKey,
						  user->usmUserAuthKeyLength));

    if (user->usmUserPrivProtocol == SNMPv3_usmNoPrivProtocol)
    {
      newRow->get_nth(7)->replace_value(new Oid(oidUsmNoPrivProtocol));
    }
    else
    {
      Oid *privoid = new Oid(priv_base);
      *privoid += user->usmUserPrivProtocol;
      newRow->get_nth(7)->replace_value(privoid);
    }

    newRow->get_nth(8)->replace_value(new OctetStr(user->usmUserPrivKey,
						  user->usmUserPrivKeyLength));
    newRow->get_nth(9)->replace_value(new OctetStr(user->usmUserPrivKey,
						  user->usmUserPrivKeyLength));

    newRow->get_nth(10)->replace_value(new OctetStr(""));
    newRow->get_nth(11)->replace_value(new SnmpInt32(2));
    newRow->get_nth(12)->replace_value(new SnmpInt32(rowActive));

    delete [] user->usmUserEngineID;
    delete [] user->usmUserName;
    delete [] user->usmUserSecurityName;
    delete user;
  }

  // register for callbacks in USM
  usm_user_table_ptr = this;
  usm->add_user_added_callback(usm_callback);
}

UsmUserTable::~UsmUserTable()
{
}

boolean UsmUserTable::ready_for_service(Vbx* pvbs, int sz)
{
  // check if cloneFrom ("4") was set:
  Oidx o;
  pvbs[3].get_value(o);
  if (o == "0.0")
    return FALSE;

  return TRUE;
}

void UsmUserTable::initialize_key_change(MibTableRow* row) 
{
  Oidx o;
  row->get_nth(4)->get_value(o);
      
  UsmKeyChange *ukc5 = (UsmKeyChange*)row->get_nth(5);
  UsmKeyChange *ukc6 = (UsmKeyChange*)row->get_nth(6);
  UsmKeyChange *ukc8 = (UsmKeyChange*)row->get_nth(8);
  UsmKeyChange *ukc9 = (UsmKeyChange*)row->get_nth(9);
  
  if ((o == oidUsmNoAuthProtocol) ||
      (auth_base.len() + 1 != o.len()) ||
      (!auth_base.is_root_of(o)))
  {
    ukc5->initialize(0, 0, AUTHKEY, ukc6);
    ukc6->initialize(0, 0, AUTHKEY, ukc5);
    ukc8->initialize(0, 0, PRIVKEY, ukc9);
    ukc9->initialize(0, 0, PRIVKEY, ukc8);
  }
  else
  {
    unsigned long auth_prot = o.last();

    Auth *auth = usm->get_auth_priv()->get_auth(auth_prot);
    int hashlength = 0;
    if (auth)
      hashlength = auth->get_hash_len();
    else
    {
      LOG_BEGIN(ERROR_LOG | 1);
      LOG("Unknown auth protocol");
      LOG(auth_prot);
      LOG_END;
    }

    ukc5->initialize(hashlength, auth_prot, AUTHKEY, ukc6);
    ukc6->initialize(hashlength, auth_prot, AUTHKEY, ukc5);
    ukc8->initialize(hashlength, auth_prot, PRIVKEY, ukc9);
    ukc9->initialize(hashlength, auth_prot, PRIVKEY, ukc8);
  }
}

void UsmUserTable::row_init(MibTableRow* new_row, const Oidx& ind, MibTable*)
{
	initialize_key_change(new_row);
	// add user to USM
	OctetStr engineID, userName, secName, authKey, privKey;
	new_row->get_nth(0)->get_value(engineID);
	new_row->get_nth(1)->get_value(userName);
	new_row->get_nth(2)->get_value(secName);
	Oidx authProtocol, privProtocol;
	new_row->get_nth(4)->get_value(authProtocol);
	new_row->get_nth(5)->get_value(authKey);
	new_row->get_nth(7)->get_value(privProtocol);
	new_row->get_nth(8)->get_value(privKey);
	long int auth = authProtocol.last();
	long int priv = privProtocol.last();
	usm->add_localized_user(engineID, userName, secName,
				   auth, authKey, priv, privKey);
}

void UsmUserTable::row_added(MibTableRow* new_row, const Oidx& ind, MibTable*)
{
  LOG_BEGIN(DEBUG_LOG | 1);
  LOG("UsmUserTable: add row with index");
  LOG(ind.get_printable());
  LOG_END;

  Oidx o = Oidx(ind);
  MibLeaf* ml;

  // set usmUserEngineID
  o = o.cut_right(o[o[0]+1]+1);
  o = o.cut_left(1);
  ml = new_row->get_nth(0);
  ml->set_value(o.as_string());

  // set usmUserName
  o = ind;
  o = o.cut_left(o[0] + 1 + 1);
  ml = new_row->get_nth(1);
  ml->set_value(o.as_string());

  // set usmSecurityName
  o = ind;
  o = o.cut_left(o[0] + 1 + 1);
  ml = new_row->get_nth(2);
  ml->set_value(o.as_string());
}

boolean UsmUserTable::addNewRow(const OctetStr& userName,
                                int authProtocol,
                                int privProtocol,
                                const OctetStr& authPassword,
                                const OctetStr& privPassword)
{
  OctetStr engineID(usm->get_local_engine_id());

  unsigned char privKey[SNMPv3_USM_MAX_KEY_LEN];
  unsigned char authKey[SNMPv3_USM_MAX_KEY_LEN];
  unsigned int authKeyLength = 0, privKeyLength = 0;

  int res = usm->get_auth_priv()->password_to_key_auth(
                                            authProtocol,
                                            authPassword.data(),
                                            authPassword.len(),
                                            engineID.data(), engineID.len(),
                                            authKey, &authKeyLength);
  if (res != SNMPv3_USM_OK)
  {
    if (res == SNMPv3_USM_UNSUPPORTED_AUTHPROTOCOL)
    {
      LOG_BEGIN(ERROR_LOG | 1);
      LOG("UsmUserTable: Unsupported authProtocol");
      LOG(authProtocol);
      LOG_END;
    }
    else
    {
      LOG_BEGIN(ERROR_LOG | 1);
      LOG("UsmUserTable: Cant add User (Errorcode)");
      LOG(res);
      LOG_END;
    }
    return FALSE;
  }

  res = usm->get_auth_priv()->password_to_key_priv(authProtocol,
                                               privProtocol,
                                               privPassword.data(),
                                               privPassword.len(),
                                               engineID.data(), engineID.len(),
                                               privKey, &privKeyLength);
  if (res != SNMPv3_USM_OK)
  {
    if (res == SNMPv3_USM_UNSUPPORTED_PRIVPROTOCOL)
    {
      LOG_BEGIN(ERROR_LOG | 1);
      LOG("UsmUserTable: Unsupported privProtocol");
      LOG(privProtocol);
      LOG_END;
    }
    else
    {
      LOG_BEGIN(ERROR_LOG | 1);
      LOG("UsmUserTable: Cant add User (Errorcode)");
      LOG(res);
      LOG_END;
    }          
    return FALSE;
  }

  // add User into MIB
  if (!addNewRow(engineID, userName, 
                 authProtocol, OctetStr(authKey, authKeyLength),
                 privProtocol, OctetStr(privKey, privKeyLength)))
     return FALSE;

  // add passwords for user to USM for discovery
  if (usm->add_usm_user(userName, authProtocol, privProtocol,
			   authPassword, privPassword)
       != SNMPv3_USM_OK)
  {
    deleteRow(engineID, userName);
    return FALSE;
  }
  return TRUE;
}

boolean UsmUserTable::addNewRow(const OctetStr& engineID,
				const OctetStr& userName,
				int authProtocol,
				const OctetStr& authKey,
                                int privProtocol,
				const OctetStr& privKey,
                                const boolean add_to_usm)
{
  return addNewRow(engineID, userName, userName,
                   authProtocol, authKey,
                   privProtocol, privKey,
                   add_to_usm);
}

boolean UsmUserTable::addNewRow(const OctetStr& engineID,
				const OctetStr& userName,
                                const OctetStr& securityName,
				int authProtocol,
				const OctetStr& authKey,
                                int privProtocol,
				const OctetStr& privKey,
                                const boolean add_to_usm)
{
  Oidx newIndex = Oidx::from_string(engineID, TRUE);
  newIndex += Oidx::from_string(userName, TRUE);

  if (add_to_usm)
  {
    if (usm->add_localized_user(engineID, userName, securityName,
				authProtocol, authKey, privProtocol, privKey)
	!= SNMPv3_USM_OK)
    {
      return FALSE;
    }
    else
    {
      LOG_BEGIN(DEBUG_LOG | 13);
      LOG("Added user to USM (engine_id) (user_name)");
      LOG(engineID.get_printable());
      LOG(userName.get_printable());
      LOG_END;
      ; // no log
    }
  }

  // if row exists: delete old row and add the new 
  if (find_index(newIndex))
    remove_row(newIndex);

  MibTableRow *newRow = add_row(newIndex);

  newRow->get_nth(2)->replace_value(securityName.clone());
  newRow->get_nth(3)->replace_value(newIndex.clone());

  UsmKeyChange *ukc5 = (UsmKeyChange*)newRow->get_nth(5);
  UsmKeyChange *ukc6 = (UsmKeyChange*)newRow->get_nth(6);
  UsmKeyChange *ukc8 = (UsmKeyChange*)newRow->get_nth(8);
  UsmKeyChange *ukc9 = (UsmKeyChange*)newRow->get_nth(9);
  
  if (authProtocol == SNMPv3_usmNoAuthProtocol)
  {
    newRow->get_nth(4)->replace_value(new Oid(oidUsmNoAuthProtocol));
    ukc5->initialize(0, 0, AUTHKEY, ukc6);
    ukc6->initialize(0, 0, AUTHKEY, ukc5);
    ukc8->initialize(0, 0, PRIVKEY, ukc9);
    ukc9->initialize(0, 0, PRIVKEY, ukc8);
  }
  else
  {
    Oid *authoid = new Oid(auth_base);
    *authoid += authProtocol;
    newRow->get_nth(4)->replace_value(authoid);

    Auth *auth = usm->get_auth_priv()->get_auth(authProtocol);
    int hashlength = 0;
    if (auth)
      hashlength = auth->get_hash_len();
    else
    {
      LOG_BEGIN(ERROR_LOG | 1);
      LOG("Unknown auth protocol");
      LOG(authProtocol);
      LOG_END;
    }

    ukc5->initialize(hashlength, authProtocol, AUTHKEY, ukc6);
    ukc6->initialize(hashlength, authProtocol, AUTHKEY, ukc5);
    ukc8->initialize(hashlength, authProtocol, PRIVKEY, ukc9);
    ukc9->initialize(hashlength, authProtocol, PRIVKEY, ukc8);

    newRow->get_nth(5)->replace_value(authKey.clone());
    newRow->get_nth(6)->replace_value(authKey.clone());
  }

  if (privProtocol == SNMPv3_usmNoPrivProtocol)
  {
    newRow->get_nth(7)->replace_value(new Oid(oidUsmNoPrivProtocol));
  }
  else
  {
    Oid *privoid = new Oid(priv_base);
    *privoid += privProtocol;
    newRow->get_nth(7)->replace_value(privoid);
    newRow->get_nth(8)->replace_value(privKey.clone());
    newRow->get_nth(9)->replace_value(privKey.clone());
  }
  newRow->get_nth(10)->replace_value(new OctetStr(""));
  newRow->get_nth(11)->replace_value(new SnmpInt32(2));
  newRow->get_nth(12)->replace_value(new SnmpInt32(rowActive));

  return TRUE;
}

boolean UsmUserTable::deleteRow(const OctetStr& engineID,
                                const OctetStr& userName)
{
  Oidx newIndex = Oidx::from_string(engineID, TRUE);
  newIndex += Oidx::from_string(userName, TRUE);

  if (!find_index(newIndex))
    return FALSE; // no such user

  if (usm->delete_localized_user(engineID, userName) != SNMPv3_USM_OK)
    return FALSE;

  remove_row(newIndex);
  return TRUE;
}

MibTableRow* UsmUserTable::get_row(Oidx o)
{
  return find_index(o);
}


UsmCloneFrom::UsmCloneFrom(Oidx o)
  : MibLeaf(o, READCREATE, new Oidx("0.0"), VMODE_DEFAULT)
{
  v3MP *v3mp = v3MP::I;
  if (!v3mp)
  {
    LOG_BEGIN(ERROR_LOG | 0);
    LOG("MPDGroup: v3MP must be initialized before the UsmUserTable");
    LOG_END;
  }

  usm = v3mp->get_usm();
}

void UsmCloneFrom::get_request(Request* req, int ind)
{

  // getRequest to this object returns 0.0
  Vbx vb(req->get_oid(ind));
  Oid o("0.0");
  vb.set_value(o);

  if (get_access() >= READONLY) {
			
    req->finish(ind, vb);
  }
  else {
    req->error(ind, SNMP_ERROR_NO_ACCESS);
  }
}


int UsmCloneFrom::prepare_set_request(Request* req, int& ind)
{
  if (get_access() >= READWRITE) {
    if (value->get_syntax() == req->get_value(ind).get_syntax()) {
      if (value_ok(req->get_value(ind))) {
	Oidx o, base = oidUsmUserEntry;
	req->get_value(ind).get_value(o);
	o = o.cut_left(base.len() + 1);

	LOG_BEGIN(DEBUG_LOG | 1);
	LOG("UsmCloneFrom::prepare_set_request: Clone from index ");
	LOG(o.get_printable());
	LOG("current value");
	LOG(((Oid*)value)->get_printable());
	LOG_END;
 
	MibTableRow * cloneRow = ((UsmUserTable*)my_table)->get_row(o);

	if (cloneRow) // check if row is active
	  if (cloneRow->get_nth(12)) {
	    int val;
	    cloneRow->get_nth(12)->get_value(val);
	    if (val==rowActive)
	      return SNMP_ERROR_SUCCESS;
	  }
	
	LOG_BEGIN(DEBUG_LOG | 1);
	LOG("UsmCloneFrom: prepare_set_request: row not found or not active");
	LOG_END;	
	return SNMP_ERROR_INCONSIS_NAME;
      }
      else return SNMP_ERROR_WRONG_VALUE;
    }
    else return SNMP_ERROR_WRONG_TYPE;
  }
  return SNMP_ERROR_NO_ACCESS;
}



int UsmCloneFrom::set(const Vbx& vb)
{
  LOG_BEGIN(DEBUG_LOG | 1);
  LOG("UsmCloneFrom::set called");
  LOG_END;	

  undo = value->clone();  
  if (vb.valid() && (vb.get_oid() == get_oid()))
    if (vb.get_syntax() == get_syntax()) {

      if ((((Oid*)value)->len() != 2) || // CloneFrom was set
	  ((*(Oid*)value)[0]!=0) ||       // before ==>
	  ((*(Oid*)value)[1]!=0)) {       // do nothing
	LOG_BEGIN(DEBUG_LOG | 1);
	LOG("UsmCloneFrom: clonefrom can be called only once");
	LOG_END;	

	return SNMP_ERROR_SUCCESS;       // and return success!
      }	  

      Oidx o, base = oidUsmUserEntry;
      vb.get_value(o);
      o = o.cut_left(base.len()+1);

      MibTableRow * cloneRow = ((UsmUserTable*)my_table)->get_row(o);
      if (!cloneRow) return SNMP_ERROR_INCONSIS_NAME;

      Oid tmpoid;
      OctetStr tmpos;
      cloneRow->get_nth(4)->get_value().get_value(tmpoid);
      my_row->get_nth(4)->set_value(tmpoid);
      cloneRow->get_nth(5)->get_value().get_value(tmpos);
      my_row->get_nth(5)->set_value(tmpos);
      cloneRow->get_nth(6)->get_value().get_value(tmpos);
      my_row->get_nth(6)->set_value(tmpos);
      cloneRow->get_nth(7)->get_value().get_value(tmpoid);
      my_row->get_nth(7)->set_value(tmpoid);
      cloneRow->get_nth(8)->get_value().get_value(tmpos);
      my_row->get_nth(8)->set_value(tmpos);
      cloneRow->get_nth(9)->get_value().get_value(tmpos);
      my_row->get_nth(9)->set_value(tmpos);

      Oidx auth_oid;
      my_row->get_nth(4)->get_value(auth_oid);
      
      UsmKeyChange *ukc5 = (UsmKeyChange*)my_row->get_nth(5);
      UsmKeyChange *ukc6 = (UsmKeyChange*)my_row->get_nth(6);
      UsmKeyChange *ukc8 = (UsmKeyChange*)my_row->get_nth(8);
      UsmKeyChange *ukc9 = (UsmKeyChange*)my_row->get_nth(9);

      if ((auth_oid == oidUsmNoAuthProtocol) ||
	  (UsmUserTable::auth_base.len() + 1 != auth_oid.len()) ||
	  (!UsmUserTable::auth_base.is_root_of(auth_oid))) {
	ukc5->initialize(0, 0, AUTHKEY, ukc6);
	ukc6->initialize(0, 0, AUTHKEY, ukc5);
	ukc8->initialize(0, 0, PRIVKEY, ukc9);
	ukc9->initialize(0, 0, PRIVKEY, ukc8);
      }
      else
      {
	unsigned long auth_prot = auth_oid.last();

	Auth *auth = usm->get_auth_priv()->get_auth(auth_prot);
	int hashlength = 0;
	if (auth)
	  hashlength = auth->get_hash_len();
	else
	{
	  LOG_BEGIN(ERROR_LOG | 1);
	  LOG("BUG: have row with unknown auth protocol");
	  LOG(auth_prot);
	  LOG_END;
	}
	ukc5->initialize(hashlength, auth_prot, AUTHKEY, ukc6);
	ukc6->initialize(hashlength, auth_prot, AUTHKEY, ukc5);
	ukc8->initialize(hashlength, auth_prot, PRIVKEY, ukc9);
	ukc9->initialize(hashlength, auth_prot, PRIVKEY, ukc8);
      }

      // change value to indicate that leaf was set

      LOG_BEGIN(DEBUG_LOG | 1);
      LOG("UsmCloneFrom: set success.");
      LOG_END;
      
      return set_value(vb);
    }
    else return SNMP_ERROR_WRONG_TYPE;
  
  else return SNMP_ERROR_BAD_VALUE;
  
}


boolean UsmCloneFrom::value_ok(const Vbx& vb)
{
  // check if row exists
  Oidx o;
  Oidx base = oidUsmUserEntry;

  if (vb.get_value(o) != SNMP_CLASS_SUCCESS)
    return FALSE;

  Oidx n("0.0");
  if (o == n) return TRUE;

  if ((o.len() < base.len()+ 3 ) ||
      (o.cut_right(o.len()-base.len()) != base)) {
    LOG_BEGIN(DEBUG_LOG | 1);
    LOG("UsmCloneFrom::value_ok: wrong length of oid or wrong base");
    LOG(o.get_printable());
    LOG_END;
    return FALSE;
  }
  o = o.cut_left(base.len());
  // RowPointer has to point to first accessible object 
  if (o[0] != 3) {
    LOG_BEGIN(DEBUG_LOG | 1);
    LOG("UsmCloneFrom: Oid should point to first accessible Object (3), but value is");
    LOG(o[0]);
    LOG_END;
    return FALSE;
  }

  /* commented out because of SilverCreek test 4.13.3 
  o = o.cut_left(1);
  if (o.len() != 1 + o[0] + o[ o[0] +1 ] + 1) {
    LOG_BEGIN(DEBUG_LOG | 1);
    LOG("UsmCloneFrom::value_ok: wrong length of oid");
    LOG(o.get_printable());
    LOG_END;
    return FALSE;
  }
  */
  return TRUE;
}

MibEntryPtr UsmCloneFrom::clone()
{
  MibEntryPtr other = new UsmCloneFrom(oid);
  // don't clone value, instead set default value
  ((UsmCloneFrom*)other)->replace_value(new Oid("0.0"));
  ((UsmCloneFrom*)other)->set_reference_to_table(my_table);
  return other;
}




UsmKeyChange::UsmKeyChange(Oidx o, int keylen, int hashfunction, int typeOfKey,
			   UsmKeyChange* ukc, USM *u)
  : MibLeaf( o, READCREATE, new OctetStr(""), VMODE_DEFAULT),
    type_of_key          (typeOfKey),
    key_len              (keylen),
    hash_function        (hashfunction),
    otherKeyChangeObject (ukc),
    usm                  (u)
{
}

UsmKeyChange::UsmKeyChange(Oidx o, USM *u)
  : MibLeaf( o, READCREATE, new OctetStr(""), VMODE_DEFAULT),
    type_of_key          (NOKEY),
    key_len              (-1),
    hash_function        (-1),
    otherKeyChangeObject (0),
    usm                  (u)
{
}

void UsmKeyChange::initialize(int keylen, int hashfunction,
                              int typeOfKey, UsmKeyChange* ukc)
{
  type_of_key = typeOfKey;
  otherKeyChangeObject = ukc;  
  key_len = keylen;
  hash_function = hashfunction;
}

UsmKeyChange::~UsmKeyChange()
{

}

void UsmKeyChange::get_request(Request* req, int ind)
{
  Vbx vb(req->get_oid(ind));
  vb.set_value("");

  if (get_access() >= READONLY) {
			
    req->finish(ind, vb);
  }
  else {
    req->error(ind, SNMP_ERROR_NO_ACCESS);
  }
}


int UsmKeyChange::prepare_set_request(Request* req, int& ind)
{
  if (get_access() >= READWRITE) {
    if (value->get_syntax() == req->get_value(ind).get_syntax()) {

      if (key_len < 0) {
	  Oidx cloneFromOID(oidUsmUserEntry);
	  cloneFromOID += 4; // cloneFrom
	  cloneFromOID += my_row->get_index();
	  Vbx* vb = req->search_value(cloneFromOID); 
	  if ((hash_function == -1) &&
	      (!vb)) {
	      delete vb;
	      return SNMP_ERROR_INCONSIS_NAME;
	  }
	  delete vb;
	  return SNMP_ERROR_SUCCESS; // can't be check here...
      }
      else
      {
        // check key length
        OctetStr os;
        Vbx vb(req->get_value(ind));
        if (vb.get_value(os) == SNMP_CLASS_INVALID)
          return SNMP_ERROR_WRONG_TYPE;
        if ((int)os.len() != 2*key_len) { // Fixed key_len
          LOG_BEGIN(DEBUG_LOG | 1);
          LOG("Keychange value has wrong length (len) (expected)");
          LOG(os.len());
          LOG(2*key_len);
          LOG_END;	
          return SNMP_ERROR_WRONG_LENGTH;
        }
        if (((OctetStr*)value)->len()==0)
          return SNMP_ERROR_INCONSIS_NAME;
        if (value_ok(req->get_value(ind)))
          return SNMP_ERROR_SUCCESS;
        else return SNMP_ERROR_WRONG_VALUE;
      }
    }
    else return SNMP_ERROR_WRONG_TYPE;
  }
  return SNMP_ERROR_NO_ACCESS;
}

int UsmKeyChange::set(const Vbx& vb)
{

  undo = value->clone();  
  if (vb.valid() && (vb.get_oid() == get_oid()))
    if (vb.get_syntax() == get_syntax()) {

      OctetStr os;
      vb.get_value(os);
      LOG_BEGIN(DEBUG_LOG | 1);
      LOG("UsmKeyChange: set: (str)");
      LOG(os.get_printable());
      LOG_END;

      if (process_key_change(os) == TRUE) {
	//todo: LOG sollte wieder entfernt werden ;-)
	LOG_BEGIN(DEBUG_LOG | 2);
	LOG("UsmKeyChange: set new key to ");
	LOG(value->get_printable());
	LOG_END;
        int stat;
        my_row->get_nth(12)->get_value().get_value(stat);
        if (stat == rowActive) {
          LOG_BEGIN(DEBUG_LOG | 1);
          LOG("UsmKeyChange: Updating Key in USM");
          LOG_END;
         
          OctetStr engineID, userName;
          my_row->get_nth(0)->get_value().get_value(engineID);
          my_row->get_nth(1)->get_value().get_value(userName);
          if (usm->update_key(userName.data(), userName.len(),
				 engineID.data(), engineID.len(),
				 ((OctetStr*)value)->data(),
				 ((OctetStr*)value)->len(),
				 type_of_key) != SNMPv3_USM_OK) {
            LOG_BEGIN( ERROR_LOG | 1 );
            LOG("UsmKeyChange: Could not update key in USM!");
            LOG_END;
            //todo: was gibt man da zurueck?
            return SNMP_ERROR_INCONSIST_VAL;
          }
        }
	// update Key in other KeyChange Object
	otherKeyChangeObject->replace_value(value->clone());
        return SNMP_ERROR_SUCCESS;
      }
      else 
        return SNMP_ERROR_BAD_VALUE;
    }
    else return SNMP_ERROR_WRONG_TYPE;
  
  else return SNMP_ERROR_BAD_VALUE;
}

int UsmKeyChange::unset()
{
  if (undo) {
    if (!otherKeyChangeObject) {
      delete undo;
      undo = 0;
      // if there is nothing to undo -> return success
      // if value can be unset (see end of method)
    }
    else {
	// unset Key in other KeyChange Object
	otherKeyChangeObject->replace_value(undo->clone());
	int stat;
	my_row->get_nth(12)->get_value().get_value(stat);
	if (stat  == rowActive) {
	    LOG_BEGIN(DEBUG_LOG | 1);
	    LOG("UsmKeyChange: undo key update in USM");
	    LOG_END;
	    OctetStr engineID, userName;
	    my_row->get_nth(0)->get_value().get_value(engineID);
	    my_row->get_nth(1)->get_value().get_value(userName);
	    if (usm->update_key(userName.data(), userName.len(),
				   engineID.data(), engineID.len(),
				   ((OctetStr*)undo)->data(),
				   ((OctetStr*)undo)->len(),
				   type_of_key) != SNMPv3_USM_OK) {
		LOG_BEGIN( ERROR_LOG | 1 );
		LOG("UsmKeyChange: Could not unset key in USM!");
		LOG_END;
		return SNMP_ERROR_UNDO_FAIL;
	    }
	}
	else {
	    // delete User in USM 
	    OctetStr engineID, userName;
	    my_row->get_nth(0)->get_value().get_value(engineID);
	    my_row->get_nth(1)->get_value().get_value(userName);
	    usm->delete_localized_user(engineID, userName);
	}
    }
  }
  return MibLeaf::unset();
}



boolean UsmKeyChange::value_ok(const Vbx& vb)
{
  OctetStr os;
  if (vb.get_value(os) == SNMP_CLASS_INVALID)
    return FALSE;

  LOG_BEGIN(DEBUG_LOG | 1);
  LOG("UsmKeyChange: value_ok (len) (key_len) ");
  LOG(os.len());
  LOG(key_len);
  LOG_END;

  return TRUE;
}
  
boolean UsmKeyChange::process_key_change(OctetStr& os)
{
  OctetStr old_key;
  old_key.set_data(((OctetStr*)value)->data(), ((OctetStr*)value)->len());

  //todo: LOG sollte wieder entfernt werden ;-)
  LOG_BEGIN(DEBUG_LOG | 1);
  LOG("UsmKeyChange: old key is ");
  LOG(old_key.get_printable());
  LOG_END;

  // append random component to old_key and hash over it
  for (int i=0; i< key_len; i++)
    old_key += os[i];

  switch (hash_function) {
    case SNMPv3_usmHMACMD5AuthProtocol: {
      unsigned char digest[16];
      AuthMD5 auth_md5;

      auth_md5.hash(old_key.data(), old_key.len(), digest);
      // XOR digest with localized new key
      for (int i = 0; i<16; i++) 
        digest[i] ^= os[key_len+i];
      ((OctetStr*)value)->set_data(digest, 16);
      break;
    }
    case SNMPv3_usmHMACSHAAuthProtocol: {
      unsigned char digest[20];
      AuthSHA auth_sha;

      auth_sha.hash(old_key.data(), old_key.len(), digest);
      // XOR digest with random-component
      for (int i = 0; i<20; i++) 
        digest[i] ^= os[key_len+i];
      ((OctetStr*)value)->set_data(digest, 20);
      break;
    }
    case -1: {
      LOG_BEGIN(ERROR_LOG | 1);
      LOG("UsmKeyChange: not initialized.");
      LOG_END;
      //      delete value;
      return FALSE;
    }
    default: {
      LOG_BEGIN(ERROR_LOG | 1);
      LOG("UsmKeyChange: Key change requested, but user doesn�t support authentification.");
      LOG_END;
      //      delete value;
      return FALSE;
    }
  }
  return TRUE;
}

MibEntryPtr UsmKeyChange::clone()
{
  MibEntryPtr other = new UsmKeyChange(oid, key_len, hash_function,
				       type_of_key, otherKeyChangeObject,
				       usm);
  ((UsmKeyChange*)other)->replace_value(value->clone());
  ((UsmKeyChange*)other)->set_reference_to_table(my_table);
  return other;
}

UsmOwnKeyChange::~UsmOwnKeyChange()
{

}

MibEntryPtr UsmOwnKeyChange::clone()
{
  MibEntryPtr other = new UsmOwnKeyChange(oid, key_len, hash_function,
					  type_of_key, otherKeyChangeObject,
					  usm);
  ((UsmOwnKeyChange*)other)->replace_value(value->clone());
  ((UsmOwnKeyChange*)other)->set_reference_to_table(my_table);
  return other;
}

int UsmOwnKeyChange::prepare_set_request(Request* req, int& ind)
{
  OctetStr security_name;
  req->get_security_name(security_name);
  LOG_BEGIN(DEBUG_LOG | 1);
  LOG("UsmOwnKeyChange: prepare_set_request: (security_name) ");
  LOG(security_name.get_printable());
  LOG_END;
  
  OctetStr my_user;
  my_row->get_nth(2)->get_value().get_value(my_user);

  if ( (my_user.len() == security_name.len()) &&
       (my_user == security_name))
    return UsmKeyChange::prepare_set_request(req, ind);
  return SNMP_ERROR_NO_ACCESS;

}

/**********************************************************************
 *  
 *  class UsmStatsUnsupportedSecLevels
 * 
 **********************************************************************/

UsmStatsUnsupportedSecLevels::UsmStatsUnsupportedSecLevels(const USM *u)
  : MibLeaf( oidUsmStatsUnsupportedSecLevels, READONLY, new Counter32(0)),
    usm(u)
{
}

void UsmStatsUnsupportedSecLevels::get_request(Request* req, int index)
{
  *((Counter32*)value) = usm->get_stats_unsupported_sec_levels();
  MibLeaf::get_request(req, index);
}


/**********************************************************************
 *  
 *  class UsmStatsNotInTimeWindows
 * 
 **********************************************************************/

UsmStatsNotInTimeWindows::UsmStatsNotInTimeWindows(const USM *u)
  : MibLeaf( oidUsmStatsNotInTimeWindows, READONLY, new Counter32(0)),
    usm(u)
{
}

void UsmStatsNotInTimeWindows::get_request(Request* req, int index)
{
  *((Counter32*)value) = usm->get_stats_not_in_time_windows();
  MibLeaf::get_request(req, index);
}


/**********************************************************************
 *  
 *  class UsmStatsUnknownUserNames
 * 
 **********************************************************************/

UsmStatsUnknownUserNames::UsmStatsUnknownUserNames(const USM *u)
  : MibLeaf( oidUsmStatsUnknownUserNames, READONLY, new Counter32(0)),
    usm(u)
{
}

void UsmStatsUnknownUserNames::get_request(Request* req, int index)
{
  *((Counter32*)value) = usm->get_stats_unknown_user_names();
  MibLeaf::get_request(req, index);
}

/**********************************************************************
 *  
 *  class UsmStatsUnknownEngineIDs
 * 
 **********************************************************************/

UsmStatsUnknownEngineIDs::UsmStatsUnknownEngineIDs(const USM *u)
  : MibLeaf( oidUsmStatsUnknownEngineIDs, READONLY, new Counter32(0)),
    usm(u)
{
}
void UsmStatsUnknownEngineIDs::get_request(Request* req, int index)
{
  *((Counter32*)value) = usm->get_stats_unknown_engine_ids();
  MibLeaf::get_request(req, index);
}


/**********************************************************************
 *  
 *  class UsmStatsWrongDigests
 * 
 **********************************************************************/

UsmStatsWrongDigests::UsmStatsWrongDigests(const USM *u)
  : MibLeaf( oidUsmStatsWrongDigests, READONLY, new Counter32(0)),
    usm(u)
{
}
void UsmStatsWrongDigests::get_request(Request* req, int index)
{
  *((Counter32*)value) = usm->get_stats_wrong_digests();
  MibLeaf::get_request(req, index);
}


/**********************************************************************
 *  
 *  class UsmStatsDecryptionErrors
 * 
 **********************************************************************/
UsmStatsDecryptionErrors::UsmStatsDecryptionErrors(const USM *u)
  : MibLeaf( oidUsmStatsDecryptionErrors, READONLY, new Counter32(0)),
    usm(u)
{
}
void UsmStatsDecryptionErrors::get_request(Request* req, int index)
{
  *((Counter32*)value) = usm->get_stats_decryption_errors();
  MibLeaf::get_request(req, index);
}

/**********************************************************************
 *  
 *  class UsmStats
 * 
 **********************************************************************/

UsmStats::UsmStats(void): MibGroup(oidUsmStats) 
{
  v3MP *v3mp = v3MP::I;
  if (!v3mp)
  {
    LOG_BEGIN(ERROR_LOG | 0);
    LOG("MPDGroup: v3MP must be initialized before this MibGroup");
    LOG_END;
  }
  USM *usm = v3mp->get_usm();

  add(new UsmStatsUnsupportedSecLevels(usm));
  add(new UsmStatsNotInTimeWindows(usm));
  add(new UsmStatsUnknownUserNames(usm));
  add(new UsmStatsUnknownEngineIDs(usm));
  add(new UsmStatsWrongDigests(usm));
  add(new UsmStatsDecryptionErrors(usm));
}

usm_mib::usm_mib(UsmUserTable* t): MibGroup("1.3.6.1.6.3.15", "usmMIB")
{
	add(new TestAndIncr("1.3.6.1.6.3.15.1.2.1.0"));
	add(t);
}

MPDGroupSnmpUnknownSecurityModels::MPDGroupSnmpUnknownSecurityModels(const v3MP *mp)
  : MibLeaf( oidSnmpUnknownSecurityModels, READONLY, new Counter32(0)),
    v3mp(mp) {}
void MPDGroupSnmpUnknownSecurityModels::get_request(Request* req, int index)
{
  *((Counter32*)value) = v3mp->get_stats_unknown_security_models();
  MibLeaf::get_request(req, index);
}
 
MPDGroupSnmpInvalidMsgs::MPDGroupSnmpInvalidMsgs(const v3MP *mp)
  : MibLeaf( oidSnmpInvalidMsgs, READONLY, new Counter32(0)),
    v3mp(mp) {}
void MPDGroupSnmpInvalidMsgs::get_request(Request* req, int index)
{
  *((Counter32*)value) = v3mp->get_stats_invalid_msgs();
  MibLeaf::get_request(req, index);
}

MPDGroupSnmpUnknownPDUHandlers::MPDGroupSnmpUnknownPDUHandlers(const v3MP *mp)
  : MibLeaf( oidSnmpUnknownPDUHandlers, READONLY, new Counter32(0)),
    v3mp(mp) {}
void MPDGroupSnmpUnknownPDUHandlers::get_request(Request* req, int index)
{
  *((Counter32*)value) = v3mp->get_stats_unknown_pdu_handlers();
  MibLeaf::get_request(req, index);
}


MPDGroup::MPDGroup(void): MibGroup(oidMPDGroup)
{
  v3MP *v3mp = v3MP::I;
  if (!v3mp)
  {
    LOG_BEGIN(ERROR_LOG | 0);
    LOG("MPDGroup: v3MP must be initialized before this MibGroup");
    LOG_END;
  }

  add(new MPDGroupSnmpUnknownSecurityModels(v3mp));
  add(new MPDGroupSnmpInvalidMsgs(v3mp));
  add(new MPDGroupSnmpUnknownPDUHandlers(v3mp));
}

usmUserAuthProtocol::usmUserAuthProtocol(const Oidx& o, USM *u):
  MibLeaf(o, READCREATE, new Oidx(oidUsmNoAuthProtocol), VMODE_DEFAULT),
  usm(u)
{
}

boolean usmUserAuthProtocol::value_ok(const Vbx& vb)
{
	Oidx o;
	vb.get_value(o);
	if (o == oidUsmNoAuthProtocol)
	  return TRUE;
	if ((o == *(Oidx *)value) && (o.len() > 2))
	  return TRUE;
	if ((((Oidx *)value)->len() == 2) &&
	    (UsmUserTable::auth_base.len() + 1 == o.len()) &&
	    (UsmUserTable::auth_base.is_root_of(o)))
	{
	  if (usm->get_auth_priv()->get_auth(o.last()))
	    return TRUE;

	  LOG_BEGIN(INFO_LOG | 4);
	  LOG("Unknown auth protocol");
	  LOG(o.last());
	  LOG_END;
	}
	return FALSE;
}

MibEntryPtr usmUserAuthProtocol::clone()
{
  MibEntryPtr other = new usmUserAuthProtocol(oid, usm);
  ((usmUserAuthProtocol*)other)->set_reference_to_table(my_table);
  return other;
}

usmUserPrivProtocol::usmUserPrivProtocol(const Oidx& o, USM *u):
  MibLeaf(o, READCREATE, new Oidx(oidUsmNoPrivProtocol), VMODE_DEFAULT),
  usm(u)
{
}

int usmUserPrivProtocol::prepare_set_request(Request* req, int& ind) 
{
	Vbx vb(req->get_value(ind));
	Oidx o;
	vb.get_value(o);

	if (o != oidUsmNoPrivProtocol) {
	    if ((UsmUserTable::priv_base.len() + 1 != o.len()) ||
		(!UsmUserTable::priv_base.is_root_of(o)) ||
		(!usm->get_auth_priv()->get_priv(o.last())))
		return SNMP_ERROR_INCONSIST_VAL;
	    
	    if (my_row)
	    {
		Oidx auth;
		my_row->get_nth(4)->get_value(auth);
		if (auth == oidUsmNoAuthProtocol)
		    return SNMP_ERROR_INCONSIST_VAL;
	    }
	}
	return MibLeaf::prepare_set_request(req, ind);
}

boolean usmUserPrivProtocol::value_ok(const Vbx& vb)
{
	Oidx o;
	vb.get_value(o);
	if (o == oidUsmNoPrivProtocol)
	  return TRUE;
	if ((o == *(Oidx *)value) && (o.len() > 2))
	  return TRUE;
	if ((((Oidx *)value)->len() == 2) &&
	    (UsmUserTable::priv_base.len() + 1 == o.len()) &&
	    (UsmUserTable::priv_base.is_root_of(o)) &&
	    (usm->get_auth_priv()->get_priv(o.last())))
	  return TRUE;
	return FALSE;
}

MibEntryPtr usmUserPrivProtocol::clone()
{
  MibEntryPtr other = new usmUserPrivProtocol(oid, usm);
  ((usmUserPrivProtocol*)other)->set_reference_to_table(my_table);
  return other;
}

#ifdef AGENTPP_NAMESPACE
}
#endif
#endif
