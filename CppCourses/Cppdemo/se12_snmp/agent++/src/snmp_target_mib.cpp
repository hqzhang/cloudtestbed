/*_############################################################################
  _## 
  _##  snmp_target_mib.cpp  
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
#include <agent_pp/snmp_target_mib.h>
#include <agent_pp/log.h>
#include <agent_pp/List.h>
#include <agent_pp/snmp_textual_conventions.h>

#ifdef SNMP_PP_NAMESPACE
using namespace Snmp_pp;
#endif

#ifdef AGENTPP_NAMESPACE
using namespace Agentpp;
#endif

/**
 *  snmpTargetAddrTDomain
 *
 */

snmpTargetAddrTDomain::snmpTargetAddrTDomain(const Oidx& id):
   MibLeaf(id, READCREATE, new Oid(), VMODE_LOCKED)
{

}

snmpTargetAddrTDomain::~snmpTargetAddrTDomain()
{
}

MibEntryPtr snmpTargetAddrTDomain::clone()
{
	MibEntryPtr other = new snmpTargetAddrTDomain(oid);
	((snmpTargetAddrTDomain*)other)->replace_value(value->clone());
	((snmpTargetAddrTDomain*)other)->set_reference_to_table(my_table);
	return other;
}


int snmpTargetAddrTDomain::get_state() 
{
	unsigned long len = ((Oid*)value)->len();
	if ((len!=7) && (len != 9)) return 0;
	if (len == 7)
	    return (*(Oid*)value)[6];
	return 100+(*(Oid*)value)[8];
}

boolean snmpTargetAddrTDomain::value_ok(const Vbx& vb)
{
	Oid val;
	vb.get_value(val);

	LOG_BEGIN(DEBUG_LOG | 6);
	LOG("snmpTargetAddrTDomain: checking (domain)");
	LOG(val.get_printable());
	LOG_END;

	// check for "old" TDomain values
	if (val.len() == 7) {
	    if ((val[6] >= 1l) && (val[6] <= 5)) {
		return TRUE;
	    }
	}
	// check for new (TRANSPORT_ADDRESS_MIB) TDomain values
	if (val.len() == 9) {
	    if ((val[8] >= 1) && (val[8] <= 16)) {
		return TRUE;
	    }
	}
	return FALSE;
}



/**
 *  snmpTargetAddrTAddress
 *
 */

snmpTargetAddrTAddress::snmpTargetAddrTAddress(const Oidx& id):
   MibLeaf(id, READCREATE, new OctetStr(""), VMODE_LOCKED)
{

}

snmpTargetAddrTAddress::snmpTargetAddrTAddress(const Oidx& id,
					       mib_access a,
					       OctetStr* s,
					       int m):
   MibLeaf(id, a, s, m)
{
}

snmpTargetAddrTAddress::~snmpTargetAddrTAddress()
{
}

MibEntryPtr snmpTargetAddrTAddress::clone()
{
	MibEntryPtr other = new snmpTargetAddrTAddress(oid);
	((snmpTargetAddrTAddress*)other)->replace_value(value->clone());
	((snmpTargetAddrTAddress*)other)->set_reference_to_table(my_table);
	return other;
}

int snmpTargetAddrTAddress::prepare_set_request(Request* req, int& ind) 
{
	Vbx domain(my_row->get_nth(0)->get_value());
	for (int i=0; i<req->subrequests(); i++) {
		if ((i != ind) && 
		    (req->get_oid(i) == my_row->get_nth(0)->get_oid())) {
			domain = req->get_value(i);
			break;
		}
	}
	unsigned long state = 0;
	Vbx vb(req->get_value(ind));
	OctetStr val;
	vb.get_value(val);
	int length = val.len();

	Oidx o;
	domain.get_value(o);
	if ((o.len()!=7) && (o.len()!=9)) return SNMP_ERROR_INCONSIST_VAL;
	if (o.len() == 7)
	    state = o[6];
	else
	    state = o[8]+100;

	LOG_BEGIN(DEBUG_LOG | 3);
	LOG("snmpTargetAddrTAddress: checking address (len)(type)");
	LOG(length);
	LOG(state);
	LOG_END;

	switch (state) {
	    case 1: // UDP Address
	    case 101:
		if (length == 6) return SNMP_ERROR_SUCCESS;
		break;
	    case 102:
		if (length == 18) return SNMP_ERROR_SUCCESS;
		break;
	    case 105:
		if (length == 6) return SNMP_ERROR_SUCCESS;
		break;
	    case 2:
	    case 3: // OSI Address  
		if ((length == 1) || ((length >= 4) && (length <= 85)))
		    return SNMP_ERROR_SUCCESS;
		break;
	    case 4:
		if ((length >= 3) && (length <= 99))
		    return SNMP_ERROR_SUCCESS;
		break;
	    case 5:
		if (length == 12)
		    return SNMP_ERROR_SUCCESS;
		break;	    
	}	
        return SNMP_ERROR_INCONSIST_VAL;
}


UdpAddress* snmpTargetAddrTAddress::getUdpAddress()
{
	UdpAddress* address = 0;
	
	int tdomain = ((snmpTargetAddrTDomain*)
		       my_row->get_nth(0))->get_state();
	switch(tdomain) {
	    case 1: 
	    case 101:
	    case 102: {
		address = new UdpAddress();
		*address = (*(OctetStr*)value);
		return address;
	    }
	}
	return 0;
}

/**
 *  snmpTargetAddrTimeout
 *
 */



/**
 *  snmpTargetAddrRetryCount
 *
 */



/**
 *  snmpTargetAddrParams
 *
 */

snmpTargetAddrParams::snmpTargetAddrParams(const Oidx& id):
   MibLeaf(id, READCREATE, new OctetStr())
{
}

snmpTargetAddrParams::~snmpTargetAddrParams()
{
}

MibEntryPtr snmpTargetAddrParams::clone()
{
	MibEntryPtr other = new snmpTargetAddrParams(oid);
	((snmpTargetAddrParams*)other)->replace_value(value->clone());
	((snmpTargetAddrParams*)other)->set_reference_to_table(my_table);
	return other;
}

int snmpTargetAddrParams::prepare_set_request(Request* req, int& ind)
{
	// place instrumentation code (manipulating "value") here
	int status = MibLeaf::prepare_set_request(req, ind);
	if (status == SNMP_ERROR_SUCCESS) {
		OctetStr newAdminString;
		Vbx vb(req->get_value(ind));
		vb.get_value(newAdminString);
                if (newAdminString.len() == 0)
                  return SNMP_ERROR_WRONG_LENGTH;
		if (!snmpTargetParamsEntry::instance->contains(newAdminString))
			return SNMP_ERROR_INCONSIST_VAL;
	}
	return status;
}


boolean snmpTargetAddrParams::value_ok(const Vbx& vb)
{
        // Vbx cvb(vb); // zero length should return wrong length!
        // if (strlen(cvb.get_printable_value()) == 0) return FALSE;
	return TRUE;
}



/**
 *  snmpTargetAddrRowStatus
 *
 */


/**
 *  snmpTargetParamsMPModel
 *
 */



/**
 *  snmpTargetParamsSecurityModel
 *
 */



/**
 *  snmpTargetParamsSecurityName
 *
 */



/**
 *  snmpTargetParamsSecurityLevel
 *
 */



/**
 *  snmpTargetParamsRowStatus
 *
 */


/**
 *  snmpTargetAddrEntry
 *
 */

snmpTargetAddrEntry* snmpTargetAddrEntry::instance = 0;

snmpTargetAddrEntry::snmpTargetAddrEntry():
   StorageTable("1.3.6.1.6.3.12.1.2.1", iSnmpAdminString, 1)
{
	// This table object is a singleton. In order to access it use
	// the static pointer snmpTargetAddrEntry::instance.
	instance = this;

	add_col(new snmpTargetAddrTDomain("2"));
	add_col(new snmpTargetAddrTAddress("3"));
	add_col(new SnmpInt32MinMax("4", READCREATE, 1500, 
				    VMODE_DEFAULT | VMODE_LOCKED, 
				    0, 2147483647));
	add_col(new SnmpInt32MinMax("5", READCREATE, 3, 
				    VMODE_DEFAULT | VMODE_LOCKED, 
				    0, 255));
	add_col(new SnmpTagList("6"));
	add_col(new snmpTargetAddrParams("7"));
	add_storage_col(new StorageType("8", 3));
	add_col(new snmpRowStatus("9"));
}

snmpTargetAddrEntry::~snmpTargetAddrEntry()
{
	instance = 0;
}

void snmpTargetAddrEntry::set_row(MibTableRow* r, const Oidx& p0, 
				  const OctetStr& p1, 
				  int p2, int p3, 
				  const OctetStr& p4, 
				  const OctetStr& p5, 
				  int p6, int p7)
{
	r->get_nth(0)->replace_value(new Oidx(p0));
	r->get_nth(1)->replace_value(new OctetStr(p1));
	r->get_nth(2)->replace_value(new SnmpInt32(p2));
	r->get_nth(3)->replace_value(new SnmpInt32(p3));
	r->get_nth(4)->replace_value(new OctetStr(p4));
	r->get_nth(5)->replace_value(new OctetStr(p5));
	r->get_nth(6)->replace_value(new SnmpInt32(p6));
	r->get_nth(7)->replace_value(new SnmpInt32(p7));
}

MibTableRow* snmpTargetAddrEntry::add_entry(const OctetStr& name,
					    const Oidx& tdomain,
					    const OctetStr& taddress,
					    const OctetStr& taglist,
					    const OctetStr& params)
{
	start_synch();
	Oidx index = Oidx::from_string(name, FALSE);
	MibTableRow* r = find_index(index);
	if (r) {
		end_synch();
		return 0;
	}
	r = add_row(index);
	r->get_nth(0)->replace_value(new Oidx(tdomain));
	r->get_nth(1)->replace_value(new OctetStr(taddress));
	// leave default values untouched (timeout and retry count)
	r->get_nth(4)->replace_value(new OctetStr(taglist));
	r->get_nth(5)->replace_value(new OctetStr(params));
	// leave default values untouched (storage type)
	r->get_nth(7)->replace_value(new SnmpInt32(rowActive));
	end_synch();
	return r;
}


boolean snmpTargetAddrEntry::refers_to(OctetStr& searchEntry)
{
	start_synch();
	OidListCursor<MibTableRow> cur;

	for (cur.init(&content); cur.get(); cur.next()) {

		OctetStr entry;
		cur.get()->get_nth(5)->get_value(entry);

		if (strcmp(entry.get_printable_hex(), 
			   searchEntry.get_printable_hex()) == 0) {

			end_synch();
			return TRUE;
		}
	}
	end_synch();
	return FALSE;
}

Address* snmpTargetAddrEntry::get_address(MibTableRow* row) 
{
	OctetStr addrStr;
	Oidx domain;
	row->get_nth(0)->get_value(domain);
	row->get_nth(1)->get_value(addrStr);
	unsigned long targetDomain = domain.last();
	switch (targetDomain) {
	  case 1:
	  case 101:
	  case 102: {	    
	      UdpAddress* address = 
		  ((snmpTargetAddrTAddress*)row->get_nth(1))->getUdpAddress();
	    return address;
	    break;
	  }
	  default: {
		LOG_BEGIN(WARNING_LOG | 2);
		LOG("snmpTargetAddrEntry: target (domain) not supported.");
		LOG(domain.get_printable());
		LOG_END;
	  }
	}
	return 0;
}
	
#ifdef _SNMPv3
UTarget* snmpTargetAddrEntry::get_target(const OctetStr& entry,
					 snmpTargetParamsEntry* paramInfo,
					 int& secLevel)
{
	start_synch();
	MibTableRow* r = find_index(Oidx::from_string(entry, FALSE));
		
	if ((!r) || (r->get_row_status()->get() != rowActive)) {

		end_synch();

		LOG_BEGIN(WARNING_LOG | 3);
		LOG("snmpTargetAddrEntry: target addr (row) not found.");
		LOG(OctetStr(entry).get_printable());
		LOG((r) ? "no active row found" : 
		    "missing row");
		LOG_END;
		return 0;
	}
	Address* address = get_address(r);
	OctetStr params;
	r->get_nth(5)->get_value(params);
	end_synch();
	if (!address) {
		return 0; 
	}
	UTarget* target = new UTarget(*address);
	delete address;
	
	if (!paramInfo->get_target_params(params, *target, secLevel)) {
		delete target;
		return 0;
	}

	return target;
}	
#endif

List<MibTableRow>* 
snmpTargetAddrEntry::get_rows_cloned_for_tag(const OctetStr& tag) 
{
	start_synch();
	OctetStr myTag(tag);
	OidListCursor<MibTableRow> cur;
	List<MibTableRow>* list = new List<MibTableRow>();
	for (cur.init(&content); cur.get(); cur.next()) {
		snmpRowStatus* status = cur.get()->get_row_status();
		if (((!status) ||
		     ((status) && (status->get() == rowActive))) && 
		    ((((SnmpTagList*)cur.get()->get_nth(4))->
		      contains(myTag.get_printable())))) {
			list->add(new MibTableRow(*cur.get()));
		}
	}
	end_synch();
	return list;
}

boolean	snmpTargetAddrEntry::ready_for_service(Vbx* pvbs, int sz) 
{
	OctetStr params;
	pvbs[5].get_value(params);
	if (params.len() == 0)
	    return FALSE;
	if (!snmpTargetParamsEntry::instance->contains(params))
	    return FALSE;
	return TRUE;
}


/**
 *  snmpTargetParamsEntry
 *
 */

snmpTargetParamsEntry* snmpTargetParamsEntry::instance = 0;

snmpTargetParamsEntry::snmpTargetParamsEntry():
   StorageTable("1.3.6.1.6.3.12.1.3.1", iSnmpAdminString, 1)
{
	// This table object is a singleton. In order to access it use
	// the static pointer snmpTargetParamsEntry::instance.
	instance = this;

	add_col(new SnmpMessageProcessingModel("2", READCREATE, 0, 
					       VMODE_DEFAULT | VMODE_LOCKED));
	add_col(new SnmpSecurityModel("3", READCREATE, 1, 
				      VMODE_DEFAULT | VMODE_LOCKED));
	add_col(new SnmpAdminString("4", READCREATE, new OctetStr(""),
				    VMODE_LOCKED, 1, 32));
	add_col(new SnmpSecurityLevel("5", READCREATE, 1, 
				      VMODE_DEFAULT | VMODE_LOCKED));
	add_storage_col(new StorageType("6", 3));
	add_col(new snmpRowStatus("7"));
}

snmpTargetParamsEntry::~snmpTargetParamsEntry()
{
	instance = 0;
}

boolean snmpTargetParamsEntry::contains(const OctetStr& name) 
{
	OidListCursor<MibTableRow> cur;
	for (cur.init(&content); cur.get(); cur.next()) {
		Oidx index = cur.get()->get_index();
		// cut length off
		OctetStr adminString(index.as_string());
		OctetStr cname(name);

		LOG_BEGIN(DEBUG_LOG | 9);
		LOG("snmpTargetParamsEntry: contains ");
		LOG(adminString.get_printable_hex());
		LOG(cname.get_printable_hex());
		LOG_END;
		if (strcmp(cname.get_printable_hex(),
			   adminString.get_printable_hex()) == 0) {
			return TRUE;
		}
	}
	return FALSE;
}

MibTableRow* snmpTargetParamsEntry::add_entry(const OctetStr& name,
					      const int mpModel, 
					      const int secModel, 
					      const OctetStr& secName,
					      const int secLevel)
{
	start_synch();
	Oidx index = Oidx::from_string(name, FALSE);
	MibTableRow* r = find_index(index);
	if (r) {
		end_synch();
		return 0;
	}
	r = add_row(index);
	r->get_nth(0)->replace_value(new SnmpInt32(mpModel));
	r->get_nth(1)->replace_value(new SnmpInt32(secModel));
	r->get_nth(2)->replace_value(new OctetStr(secName));
	r->get_nth(3)->replace_value(new SnmpInt32(secLevel));
	// leave default value untouched (storage type)
	r->get_nth(5)->replace_value(new SnmpInt32(rowActive));
	end_synch();
	return r;	
}


#ifdef _SNMPv3
boolean snmpTargetParamsEntry::get_target_params(const OctetStr& param,
						 UTarget& target,
						 int& secLevel)
{
	start_synch();
	MibTableRow* paramsRow = find_index(Oidx::from_string(param, FALSE));
		
	if ((!paramsRow) || 
	    (paramsRow->get_row_status()->get() != rowActive)) {

		end_synch();

		LOG_BEGIN(WARNING_LOG | 3);
		LOG("snmpTargetParamsEntry: target addr parameter (row) not found.");
		LOG(OctetStr(param).get_printable());
		LOG((paramsRow) ? "no active row found" : 
		    "missing row");
		LOG_END;
		return FALSE;
	}

	int secModel, mpModel;
	OctetStr secName;

	paramsRow->get_nth(0)->get_value(mpModel);
	paramsRow->get_nth(1)->get_value(secModel);
	paramsRow->get_nth(2)->get_value(secName);
	paramsRow->get_nth(3)->get_value(secLevel);

	end_synch();
	
	switch (mpModel) {
	case 0: {
		target.set_version(version1);
		break;
	}
	case 1: {
		target.set_version(version2c);
		break;
	}
	case 2: {
		LOG_BEGIN(WARNING_LOG | 3);
		LOG("snmpTargetParamsEntry: mpModel SNMPv2u/* not supported");
		LOG_END;

		return FALSE;
		break;
	}
	case 3: {
		target.set_version(version3);
		break;
	}
	}
	target.set_security_model(secModel);
	target.set_security_name(secName);

	return TRUE;
}	
#endif

snmp_target_mib::snmp_target_mib(): MibGroup("1.3.6.1.6.3.12", "snmpTargetMIB")
{
	add(new TestAndIncr("1.3.6.1.6.3.12.1.1.0"));
	add(new snmpTargetAddrEntry());
	add(new snmpTargetParamsEntry());
}








