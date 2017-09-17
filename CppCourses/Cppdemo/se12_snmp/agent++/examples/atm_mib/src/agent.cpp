/*_############################################################################
  _## 
  _##  agent.cpp  
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

#include <stdlib.h>
#include <signal.h>

#include <agent_pp/agent++.h>
#include <agent_pp/log.h>
#include <agent_pp/snmp_group.h>
#include <agent_pp/system_group.h>
#include <agent_pp/snmp_target_mib.h>
#include <agent_pp/snmp_notification_mib.h>
#include <agent_pp/snmp_community_mib.h>
#include <agent_pp/notification_originator.h>
#include <agent_pp/notification_log_mib.h>
#include <agent_pp/agentpp_simulation_mib.h>
#include <agent_pp/agentpp_config_mib.h>
#include <agent_pp/v3_mib.h>
#include <agent_pp/mib_policy.h>
#include <agent_pp/vacm.h>

#include <snmp_pp/oid_def.h>
#include <snmp_pp/mp_v3.h>

#include "atm_mib.h"
#include "agentpp_notifytest_mib.h"
#include "agentpp_test_mib.h"


#ifdef SNMP_PP_NAMESPACE
using namespace Snmp_pp;
#endif

#ifdef AGENTPP_NAMESPACE
using namespace Agentpp;
#endif

// table size policies


// globals:

u_short port;
Mib* mib;
RequestList* reqList;
boolean run = TRUE;
#ifdef _SNMPv3
const table_size_def table_sizes[4] =
{ table_size_def(oidUsmUserEntry, 30),
  table_size_def(oidVacmSecurityToGroupEntry, 30),
  table_size_def(oidVacmAccessEntry, 30),
  table_size_def(oidVacmViewTreeFamilyEntry, 30)
};
MibTableSizePolicy policy(table_sizes, 4, 50);
#endif


static void sig(int signo)
{
	if ((signo == SIGTERM) || (signo == SIGINT) ||
	    (signo == SIGSEGV)) {

		printf ("\n");
      
		switch (signo) {
		case SIGSEGV: {
			printf ("Segmentation fault, aborting.\n");
			exit(1);
		}
		case SIGTERM: 
		case SIGINT: {
		        if (run) {
				run = FALSE;
				printf ("User abort\n");
			}
		}
		}
	}
}


void init_signals()
{
	signal (SIGTERM, sig);
	signal (SIGINT, sig);
	signal (SIGSEGV, sig); 
}	



void init(Mib& mib)
{
	OctetStr sysDescr("AGENT++v");
	sysDescr += AGENTPP_VERSION_STRING;
	sysDescr += " ATM Simulation Agent";
        mib.add(new sysGroup(sysDescr.get_printable(), 
			     "1.3.6.1.4.1.4976", 10)); 
 	mib.add(new snmpGroup());
	mib.add(new TestAndIncr(oidSnmpSetSerialNo));
	mib.add(new atm_mib());
	mib.add(new agentpp_simulation_mib());
	mib.add(new agentpp_notifytest_mib());
	mib.add(new agentpp_test_mib());
	mib.add(new snmp_target_mib());
#ifdef _SNMPv3
	mib.add(new snmp_community_mib());
#endif
	mib.add(new snmp_notification_mib());
#ifdef _SNMPv3
	mib.add(new agentpp_config_mib(&mib));
	mib.add(new notification_log_mib());

	OctetStr nonDefaultContext("other");
	mib.add(nonDefaultContext, new atm_mib());

	UsmUserTable *uut = new UsmUserTable();

	uut->addNewRow("unsecureUser",
		       SNMPv3_usmNoAuthProtocol,
		       SNMPv3_usmNoPrivProtocol, "", "");
	
	uut->addNewRow("MD5",
		       SNMPv3_usmHMACMD5AuthProtocol,
		       SNMPv3_usmNoPrivProtocol,
		       "MD5UserAuthPassword", "");
	
	uut->addNewRow("SHA",
		       SNMPv3_usmHMACSHAAuthProtocol,
		       SNMPv3_usmNoPrivProtocol,
		       "SHAUserAuthPassword", "");
	
	uut->addNewRow("MD5DES",
		       SNMPv3_usmHMACMD5AuthProtocol,
		       SNMPv3_usmDESPrivProtocol,
		       "MD5DESUserAuthPassword",
		       "MD5DESUserPrivPassword");

	uut->addNewRow("SHADES",
		       SNMPv3_usmHMACSHAAuthProtocol,
		       SNMPv3_usmDESPrivProtocol,
		       "SHADESUserAuthPassword",
		       "SHADESUserPrivPassword");
	
	uut->addNewRow("MD5IDEA",
		       SNMPv3_usmHMACMD5AuthProtocol,
		       SNMPv3_usmIDEAPrivProtocol,
		       "MD5IDEAUserAuthPassword",
		       "MD5IDEAUserPrivPassword");
	
	uut->addNewRow("SHAIDEA",
		       SNMPv3_usmHMACSHAAuthProtocol,
		       SNMPv3_usmIDEAPrivProtocol,
		       "SHAIDEAUserAuthPassword",
		       "SHAIDEAUserPrivPassword");

	uut->addNewRow("MD5AES128",
		       SNMPv3_usmHMACMD5AuthProtocol,
		       SNMPv3_usmAES128PrivProtocol,
		       "MD5AES128UserAuthPassword",
		       "MD5AES128UserPrivPassword");
	
	uut->addNewRow("SHAAES128",
		       SNMPv3_usmHMACSHAAuthProtocol,
		       SNMPv3_usmAES128PrivProtocol,
		       "SHAAES128UserAuthPassword",
		       "SHAAES128UserPrivPassword");

	uut->addNewRow("MD5AES192",
		       SNMPv3_usmHMACMD5AuthProtocol,
		       SNMPv3_usmAES192PrivProtocol,
		       "MD5AES192UserAuthPassword",
		       "MD5AES192UserPrivPassword");
	
	uut->addNewRow("SHAAES192",
		       SNMPv3_usmHMACSHAAuthProtocol,
		       SNMPv3_usmAES192PrivProtocol,
		       "SHAAES192UserAuthPassword",
		       "SHAAES192UserPrivPassword");

	uut->addNewRow("MD5AES256",
		       SNMPv3_usmHMACMD5AuthProtocol,
		       SNMPv3_usmAES256PrivProtocol,
		       "MD5AES256UserAuthPassword",
		       "MD5AES256UserPrivPassword");
	
	uut->addNewRow("SHAAES256",
		       SNMPv3_usmHMACSHAAuthProtocol,
		       SNMPv3_usmAES256PrivProtocol,
		       "SHAAES256UserAuthPassword",
		       "SHAAES256UserPrivPassword");

	// add non persistent USM statistics
	mib.add(new UsmStats());
	// add the USM MIB - usm_mib MibGroup is used to
	// make user added entries persistent
	mib.add(new usm_mib(uut));
	// add non persistent SNMPv3 engine object
	mib.add(new V3SnmpEngine());
	mib.add(new MPDGroup());
#endif
}	




int main (int argc, char* argv[])
{
	if (argc>1)
		port = atoi(argv[1]);
	else
		port = 4700;

#ifndef _NO_LOGGING
	DefaultLog::log()->set_filter(ERROR_LOG, 5);
	DefaultLog::log()->set_filter(WARNING_LOG, 5);
	DefaultLog::log()->set_filter(EVENT_LOG, 5);
	DefaultLog::log()->set_filter(INFO_LOG, 5);
	DefaultLog::log()->set_filter(DEBUG_LOG, 8);
#endif
	int status;
	Snmp::socket_startup();  // Initialize socket subsystem
#ifdef SNMP_PP_WITH_UDPADDR
	// bind localhost only -> agent will not be reachable from
	// outside
	// UdpAddress inaddr("127.0.0.1");
	UdpAddress inaddr("0.0.0.0");
	inaddr.set_port(port);
	Snmpx snmp(status, inaddr);
#else
	// bind on any address on local host
	Snmpx snmp(status, port);
#endif

	if (status == SNMP_CLASS_SUCCESS) {

		LOG_BEGIN(EVENT_LOG | 1);
		LOG("main: SNMP listen port");
		LOG(port);
		LOG_END;
	}
	else {
		LOG_BEGIN(ERROR_LOG | 0);
		LOG("main: SNMP port init failed");
		LOG(status);
		LOG_END;
		exit(1);
	}

	mib = new Mib();

#ifdef _SNMPv3
        unsigned int snmpEngineBoots = 0;
        OctetStr engineId(SnmpEngineID::create_engine_id(port));

        // you may use your own methods to load/store this counter
        status = mib->get_boot_counter(engineId, snmpEngineBoots);
        if ((status != SNMPv3_OK) && (status < SNMPv3_FILEOPEN_ERROR)) {
		LOG_BEGIN(ERROR_LOG | 0);
		LOG("main: Error loading snmpEngineBoots counter (status)");
		LOG(status);
		LOG_END;
		exit(1);
	}

        snmpEngineBoots++;
        status = mib->set_boot_counter(engineId, snmpEngineBoots);
        if (status != SNMPv3_OK) {
		LOG_BEGIN(ERROR_LOG | 0);
		LOG("main: Error saving snmpEngineBoots counter (status)");
		LOG(status);
		LOG_END;
		exit(1);
	}

	int stat;
        v3MP *v3mp = new v3MP(engineId, snmpEngineBoots, stat);
#endif
	reqList = new RequestList();
#ifdef _SNMPv3
	// register v3MP
	reqList->set_v3mp(v3mp);
#endif
	// register requestList for outgoing requests
	mib->set_request_list(reqList);

	init_signals();

	// add supported objects
	init(*mib);

	reqList->set_snmp(&snmp);

#ifdef _SNMPv3
	// register VACM
	Vacm* vacm = new Vacm(*mib);
	reqList->set_vacm(vacm);

	// initialize security information
        vacm->addNewContext("");
        vacm->addNewContext("other");

        // Add new entries to the SecurityToGroupTable.
        // Used to determine the group a given SecurityName belongs to. 
        // User "new" of the USM belongs to newGroup
	
        vacm->addNewGroup(SecurityModel_USM, "new",
                         "newGroup", storageType_nonVolatile);

        vacm->addNewGroup(SecurityModel_USM, "test", 
                         "testGroup", storageType_nonVolatile);
        vacm->addNewGroup(SecurityModel_v2, "public", 
                         "v1v2group", storageType_nonVolatile);
        vacm->addNewGroup(SecurityModel_v1, "public", 
                         "v1v2group", storageType_nonVolatile);
        vacm->addNewGroup(SecurityModel_USM, "initial", 
                         "initial", storageType_nonVolatile);
        vacm->addNewGroup(SecurityModel_USM, "unsecureUser", 
                         "newGroup", storageType_nonVolatile);
        vacm->addNewGroup(SecurityModel_USM, "MD5", 
                         "testNoPrivGroup", storageType_nonVolatile);
        vacm->addNewGroup(SecurityModel_USM, "SHA", 
                         "testNoPrivGroup", storageType_nonVolatile);
        vacm->addNewGroup(SecurityModel_USM, "MD5DES", 
                         "testGroup", storageType_nonVolatile);
        vacm->addNewGroup(SecurityModel_USM, "SHADES", 
                         "testGroup", storageType_nonVolatile);
        vacm->addNewGroup(SecurityModel_USM, "MD5IDEA", 
                         "testGroup", storageType_nonVolatile);
        vacm->addNewGroup(SecurityModel_USM, "SHAIDEA", 
                         "testGroup", storageType_nonVolatile);
        vacm->addNewGroup(SecurityModel_USM, "MD5AES128",
                         "testGroup", storageType_nonVolatile);
        vacm->addNewGroup(SecurityModel_USM, "SHAAES128", 
                         "testGroup", storageType_nonVolatile);
        vacm->addNewGroup(SecurityModel_USM, "MD5AES192",
                         "testGroup", storageType_nonVolatile);
        vacm->addNewGroup(SecurityModel_USM, "SHAAES192", 
                         "testGroup", storageType_nonVolatile);
        vacm->addNewGroup(SecurityModel_USM, "MD5AES256",
                         "testGroup", storageType_nonVolatile);
        vacm->addNewGroup(SecurityModel_USM, "SHAAES256", 
                         "testGroup", storageType_nonVolatile);
	
        // remove a group with:
        //vacm->deleteGroup(SecurityModel_USM, "neu");

        // Set access rights of groups.
        // The group "newGroup" (when using the USM with a security
	// level >= noAuthNoPriv within context "") would have full access  
        // (read, write, notify) to all objects in view "newView". 
        vacm->addNewAccessEntry("newGroup", 
				"other",        // context
				SecurityModel_USM, 
				SecurityLevel_noAuthNoPriv,
				match_exact,  // context must mach exactly
				// alternatively: match_prefix  
				"newView", // readView
				"newView", // writeView
				"newView", // notifyView
				storageType_nonVolatile);
        vacm->addNewAccessEntry("testGroup", "",
				SecurityModel_USM, SecurityLevel_authPriv, 
				match_prefix,
				"testView", "testView", 
				"testView", storageType_nonVolatile);
        vacm->addNewAccessEntry("testNoPrivGroup", "",
				SecurityModel_USM, SecurityLevel_authNoPriv, 
				match_prefix,
				"testView", "testView", 
				"testView", storageType_nonVolatile);
        vacm->addNewAccessEntry("testNoPrivGroup", "",
				SecurityModel_USM, SecurityLevel_noAuthNoPriv, 
				match_prefix,
				"testView", "testView", 
				"testView", storageType_nonVolatile);
        vacm->addNewAccessEntry("testGroup", "",
				SecurityModel_USM, SecurityLevel_noAuthNoPriv, 
				match_prefix,
				"testView", "testView", 
				"testView", storageType_nonVolatile);
        vacm->addNewAccessEntry("v1v2group", "", 
				SecurityModel_v2, SecurityLevel_noAuthNoPriv, 
				match_exact,
				"v1ReadView", "v1WriteView", 
				"v1NotifyView", storageType_nonVolatile);
        vacm->addNewAccessEntry("v1v2group", "", 
				SecurityModel_v1, SecurityLevel_noAuthNoPriv, 
				match_exact,
				"v1ReadView", "v1WriteView", 
				"v1NotifyView", storageType_nonVolatile);
        vacm->addNewAccessEntry("initial", "",
				SecurityModel_USM, SecurityLevel_noAuthNoPriv, 
				match_exact,
				"restricted", "", 
				"restricted", storageType_nonVolatile);
        vacm->addNewAccessEntry("initial", "",
				SecurityModel_USM, SecurityLevel_authNoPriv, 
				match_exact,
				"internet", "internet", 
				"internet", storageType_nonVolatile);
        vacm->addNewAccessEntry("initial", "",
				SecurityModel_USM, SecurityLevel_authPriv, 
				match_exact,
				"internet", "internet", 
				"internet", storageType_nonVolatile);

        // remove an AccessEntry with:
        // vacm->deleteAccessEntry("newGroup", 
	//	      		"",        
	//			SecurityModel_USM, 
	//			SecurityLevel_noAuthNoPriv);


        // Defining Views
        // View "v1ReadView" includes all objects starting with "1.3".
        // If the ith bit of the mask is not set (0), then also all objects
	// which have a different subid at position i are included in the 
	// view.
        // For example: Oid "6.5.4.3.2.1", Mask(binary) 110111 
        //              Then all objects with Oid with "6.5.<?>.3.2.1" 
	//              are included in the view, whereas <?> may be any
	//              natural number.

        vacm->addNewView("v1ReadView", 
			 "1.3",       
			 "",             // Mask "" is same as 0xFFFFFFFFFF...
			 view_included,  // alternatively: view_excluded
			 storageType_nonVolatile);

        vacm->addNewView("v1WriteView", 
			 "1.3",       
			 "",             // Mask "" is same as 0xFFFFFFFFFF...
			 view_included,  // alternatively: view_excluded
			 storageType_nonVolatile);

        vacm->addNewView("v1NotifyView", 
			 "1.3",       
			 "",             // Mask "" is same as 0xFFFFFFFFFF...
			 view_included,  // alternatively: view_excluded
			 storageType_nonVolatile);

        vacm->addNewView("newView", "1.3", "", 
			 view_included, storageType_nonVolatile);
        vacm->addNewView("testView", "1.3.6", "",
			 view_included, storageType_nonVolatile);
        vacm->addNewView("internet", "1.3.6.1","",
			 view_included, storageType_nonVolatile);
	
        vacm->addNewView("restricted", "1.3.6.1.2.1.1","",
			 view_included, storageType_nonVolatile);
        vacm->addNewView("restricted", "1.3.6.1.2.1.11","", 
			 view_included, storageType_nonVolatile);
        vacm->addNewView("restricted", "1.3.6.1.6.3.10.2.1","", 
			 view_included, storageType_nonVolatile);
        vacm->addNewView("restricted", "1.3.6.1.6.3.11.2.1","",
			 view_included, storageType_nonVolatile);
        vacm->addNewView("restricted", "1.3.6.1.6.3.15.1.1","", 
			 view_included, storageType_nonVolatile);

	// add SNMPv1/v2c community to v3 security name mapping
	OctetStr co("public");
	MibTableRow* row = snmpCommunityEntry::instance->
	    add_row(Oidx::from_string(co, FALSE));
	OctetStr tag("v1v2cPermittedManagers");
	snmpCommunityEntry::instance->
	    set_row(row, co, co,
		    reqList->get_v3mp()->get_local_engine_id(),
		    "", tag, 3, 1);

#endif		

#ifdef _SNMPv3
	// register table size policies
	MibTableSizePolicy::register_policy(mib->get_default_context(), 
					    &policy);
#endif
	// load persitent objects from disk
	mib->init();
	
	
	Vbx* vbs = 0;
	coldStartOid coldOid;
	NotificationOriginator no;
	// add an example destination
	UdpAddress dest("127.0.0.1/162");
	no.add_v1_trap_destination(dest);
	// send the notification
	mib->notify("", coldOid, vbs, 0);


	Request* req;
	while (run) {
	  
		req = reqList->receive(2);

		if (req) {
		    mib->process_request(req);
		}
		else {
		    mib->cleanup();
		}
	}
	delete mib;
	Snmp::socket_cleanup();  // Shut down socket subsystem
	return 0;
}
