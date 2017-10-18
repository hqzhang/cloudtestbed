/*_############################################################################
  _## 
  _##  sim_mib.cpp  
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


#include <agent_pp/sim_mib.h>
#include <snmp_pp/target.h>
#include <agent_pp/log.h>
#include <agent_pp/snmp_request.h> 
#include <agent_pp/system_group.h>

#ifdef SNMP_PP_NAMESPACE
using namespace Snmp_pp;
#endif

#ifdef AGENTPP_NAMESPACE
namespace Agentpp {
#endif

SimMib::SimMib(): Mib()
{
}

SimMib::~SimMib() 
{
}



SimMibLeaf::SimMibLeaf(): MibLeaf() 
{
}

SimMibLeaf::SimMibLeaf(const Oidx& o, mib_access a, SmiUINT32 s): 
  MibLeaf(o, a, s)
{
}

SimMibLeaf::SimMibLeaf(const Oidx& o, mib_access a, SnmpSyntax* s): 
  MibLeaf(o, a, s)
{
}

SimMibLeaf::SimMibLeaf(const Oidx& o, mib_access a, SnmpSyntax* s, boolean d):
  MibLeaf(o, a, s, d)
{
}

SimMibLeaf::SimMibLeaf(const SimMibLeaf& other): MibLeaf(other) 
{
}

SimMibLeaf::~SimMibLeaf()
{
}

MibEntryPtr SimMibLeaf::clone()
{
	MibEntryPtr other = new SimMibLeaf(*this);
	return other;
}

mib_access SimMibLeaf::get_access()
{
	if ((configMode) && (my_table)) {
		return READCREATE;
	}
	if (configMode) return READWRITE;
	return MibLeaf::get_access();
}  

void SimMibLeaf::set_config_mode() 
{
	configMode = TRUE;
}

void SimMibLeaf::unset_config_mode() 
{
	configMode = FALSE;
}

boolean SimMibLeaf::get_config_mode() 
{
	return configMode;
}

boolean SimMibLeaf::configMode = FALSE;


int SimMibTable::check_creation(Request* req, int& ind) 
{
	if (SimMibLeaf::get_config_mode()) {
		return SNMP_ERROR_SUCCESS;
	}
	else return MibTable::check_creation(req, ind);
}

MibEntryPtr SimRowStatus::clone()
{
	MibEntryPtr other = new SimRowStatus(oid, access);
	((SimRowStatus*)other)->replace_value(value->clone());
	((SimRowStatus*)other)->set_reference_to_table(my_table);
	return other;
}

boolean SimRowStatus::transition_ok(const Vbx& vb)
{
	if (SimMibLeaf::get_config_mode()) {
		return TRUE;
	}
	else return snmpRowStatus::transition_ok(vb);
}

mib_access SimRowStatus::get_access()
{
	if ((SimMibLeaf::get_config_mode()) && (my_table)) {
		return READCREATE;
	}
	if (SimMibLeaf::get_config_mode()) return READWRITE;
	return MibLeaf::get_access();
}  



/**********************************************************************
 *  
 *  class simSysUpTime
 * 
 **********************************************************************/

time_t simSysUpTime::start = 0;

simSysUpTime::simSysUpTime(): SimMibLeaf(oidSysUpTime, READONLY, 
					 new TimeTicks(0))
{
	start = get_currentTime();
}

time_t simSysUpTime::get_currentTime()
{
	time_t now;
	time(&now);
	return now;
}

time_t simSysUpTime::get()
{
	return (get_currentTime() - start)*100;
}

void simSysUpTime::get_request(Request* req, int ind)
{
	*((TimeTicks*)value) = (unsigned long)get();
	MibLeaf::get_request(req, ind);
}

simSysGroup::simSysGroup(): MibGroup("1.3.6.1.2.1.1", "simSysGroup")
{
	add(new simSysUpTime());
	add(new SimMibLeaf("1.3.6.1.2.1.1.1.0", READONLY, 
			   new OctetStr(""), FALSE));
	add(new SimMibLeaf("1.3.6.1.2.1.1.2.0", READONLY, 
			   new Oid(""), FALSE));
	add(new SimMibLeaf("1.3.6.1.2.1.1.4.0", READWRITE, 
			   new OctetStr(""), FALSE));
	add(new SimMibLeaf("1.3.6.1.2.1.1.5.0", READWRITE, 
			   new OctetStr(""), FALSE));
	add(new SimMibLeaf("1.3.6.1.2.1.1.6.0", READWRITE, 
			   new OctetStr(""), FALSE));
	add(new SimMibLeaf("1.3.6.1.2.1.1.7.0", READONLY,
			   new SnmpInt32(0), FALSE));
}

#ifdef AGENTPP_NAMESPACE
}
#endif
