/*
 * Copyright 2009-2011 Red Hat, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// condor includes
#include "condor_common.h"
#include "stl_string_utils.h"

// local includes
#include "Collectables.h"
#include "AviaryConversionMacros.h"
#include "AviaryUtils.h"

using namespace std;
using namespace aviary::util;

using namespace aviary::collector;

void DaemonCollectable::update(const ClassAd& ad)
{
    MGMT_DECLARATIONS;
    DaemonCollectable& m_stats = *this;
    m_stats.Pool = getPoolName();
    STRING(Name);
    STRING(MyAddress);
    STRING(CondorPlatform);
    STRING(CondorVersion);
    INTEGER(DaemonStartTime);
}


void Collector::update(const ClassAd& ad)
{
    MGMT_DECLARATIONS;
    DaemonCollectable::update(ad);
    Collector& m_stats = *this;
    INTEGER(RunningJobs);
    INTEGER(IdleJobs);
    INTEGER(HostsTotal);
    INTEGER(HostsClaimed);
    INTEGER(HostsUnclaimed);
    INTEGER(HostsOwner);
    
}

void Master::update(const ClassAd& ad)
{
    MGMT_DECLARATIONS;
    DaemonCollectable::update(ad);
    Master& m_stats = *this;
    STRING(Arch);
    STRING(OpSysLongName);
    INTEGER(RealUid);
}

void Negotiator::update(const ClassAd& ad)
{
    MGMT_DECLARATIONS;
    DaemonCollectable::update(ad);
    Negotiator& m_stats = *this;
    DOUBLE2(LastNegotiationCycleMatchRate0,MatchRate);
    INTEGER2(LastNegotiationCycleMatches0,Matches);
    INTEGER2(LastNegotiationCycleDuration0,Duration);
    INTEGER2(LastNegotiationCycleNumSchedulers0,NumSchedulers);
    INTEGER2(LastNegotiationCycleActiveSubmitterCount0,ActiveSubmitterCount);
    INTEGER2(LastNegotiationCycleNumIdleJobs0,NumIdleJobs);
    INTEGER2(LastNegotiationCycleNumJobsConsidered0,NumJobsConsidered);
    INTEGER2(LastNegotiationCycleRejections0,Rejections);
    INTEGER2(LastNegotiationCycleTotalSlots0,TotalSlots);
    INTEGER2(LastNegotiationCycleCandidateSlots0,CandidateSlots);
    INTEGER2(LastNegotiationCycleTrimmedSlots0,TrimmedSlots);
}

void Scheduler::update(const ClassAd& ad)
{
    MGMT_DECLARATIONS;
    DaemonCollectable::update(ad);
    Scheduler& m_stats = *this;
    INTEGER(JobQueueBirthdate);
    INTEGER(MaxJobsRunning);
    INTEGER(NumUsers);
    INTEGER(TotalJobAds);
    INTEGER(TotalRunningJobs);
    INTEGER(TotalHeldJobs);
    INTEGER(TotalIdleJobs);
    INTEGER(TotalRemovedJobs);
}

void Slot::update(const ClassAd& ad)
{
    MGMT_DECLARATIONS;
    DaemonCollectable::update(ad);
    Slot& m_stats = *this;
    STRING(SlotType);
    upper_case(SlotType);
    STRING(Arch);
    STRING(OpSys);
    STRING(Activity);
    STRING(State);
    INTEGER(Cpus);
    INTEGER(Disk);
    INTEGER(Memory);
    INTEGER(Swap);
    INTEGER(Mips);
    DOUBLE(LoadAvg);
    STRING(Start);
    STRING(FileSystemDomain);
}


void Submitter::update(const ClassAd& ad)
{
    MGMT_DECLARATIONS;
    Submitter& m_stats = *this;
    STRING(Name);
    STRING(Machine);
    STRING(ScheddName);
    INTEGER(RunningJobs);
    INTEGER(HeldJobs);
    INTEGER(IdleJobs);
}
