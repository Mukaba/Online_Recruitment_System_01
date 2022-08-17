<?php
/*

 +------------------------------------------------------------------------------+
 | Mamook(R) Software                                                           |
 +------------------------------------------------------------------------------+
 | Copyright (c) 2000-2005 University of Victoria.  All rights reserved.        |
 +------------------------------------------------------------------------------+
 | THE LICENSED WORK IS PROVIDED UNDER THE TERMS OF THE ADAPTIVE PUBLIC LICENSE |
 | ("LICENSE") AS FIRST COMPLETED BY: The University of Victoria. ANY USE,      |
 | PUBLIC DISPLAY, PUBLIC PERFORMANCE, REPRODUCTION OR DISTRIBUTION OF, OR      |
 | PREPARATION OF DERIVATIVE WORKS BASED ON, THE LICENSED WORK CONSTITUTES      |
 | RECIPIENT'S ACCEPTANCE OF THIS LICENSE AND ITS TERMS, WHETHER OR NOT SUCH    |
 | RECIPIENT READS THE TERMS OF THE LICENSE. "LICENSED WORK" AND "RECIPIENT"    |
 | ARE DEFINED IN THE LICENSE. A COPY OF THE LICENSE IS LOCATED IN THE TEXT     |
 | FILE ENTITLED "LICENSE.TXT" ACCOMPANYING THE CONTENTS OF THIS FILE. IF A     |
 | COPY OF THE LICENSE DOES NOT ACCOMPANY THIS FILE, A COPY OF THE LICENSE MAY  |
 | ALSO BE OBTAINED AT THE FOLLOWING WEB SITE: http://www.mamook.net            |  
 |                                                                              |
 | Software distributed under the License is distributed on an "AS IS" basis,   |
 | WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for |
 | the specific language governing rights and limitations under the License.    | 
 +------------------------------------------------------------------------------+
 | Filename: pending_jobs.inc                                                   |
 +------------------------------------------------------------------------------+
 | Description: This file does all of the decision making for when a user (an   |
 | office user) would like to view pending jobs.  These are jobs, company, and  |
 | contact info that have recently been added by new employers, and not yet     |
 | approved.                                                                    |
 +------------------------------------------------------------------------------+

*/

include('job_descriptions/pending_jobs/PendingEmployer.class');

if (!$continue || $continue == 'Cancel')
{
	// The user just got here, so show them the list of jobs that are pending.

	if ($userlevel == OFFICE)
	{
		include('job_descriptions/pending_jobs/pending_jobs_list.inc');
	}
	elseif ($userlevel == TRAFFICDIRECTOR)
	{
		include('job_descriptions/pending_jobs/traffic_director_job_list.inc');
	}
}

elseif ($continue == 'view_job_info')
{
	/*
	 The user would like to see the pending job's information.
	*/

	assert(isset($job_id));

	$pendingEmployer = new PendingEmployer('');
	$pendingEmployer->load_pending_values($job_id);

	include('job_descriptions/pending_jobs/view_job_info.inc');
}

elseif ($continue == 'view_contact_company_info')
{
	// The user would like to see the pending contact's information.

	assert(isset($job_id));
		
	$pendingEmployer = new PendingEmployer('');
	$pendingEmployer->load_pending_values($job_id);

	include('job_descriptions/pending_jobs/view_contact_company_info.inc');
}

elseif ($continue == 'view_contact_company_info_incomplete')
{
	// The user wants to see the pending contact and company's incomplete information

	assert(isset($contact_id));

	$pendingEmployer = new PendingEmployer('');
	$pendingEmployer->load_incomplete_values($contact_id);

	include('job_descriptions/pending_jobs/view_contact_company_info_incomplete.inc');
}

elseif ($continue == 'Approve Company/Contact')
{
	// The user has decided to approve this contact and company.

	assert(isset($job_id));

	include('job_descriptions/pending_jobs/check_for_duplicates.inc');
}

elseif ($continue == 'Reject Company/Contact')
{
	// The user has rejected this company and contact info.  Confirm the rejection.

	assert(isset($job_id));

	$pendingEmployer = new PendingEmployer('');
	$pendingEmployer->load_pending_values($job_id);

	include('job_descriptions/pending_jobs/confirm_rejection.inc');
}

elseif ($continue == 'Assign Host Department')
{
	// The traffic director would like to assign a new host department for this job.

	if ($userlevel == TRAFFICDIRECTOR)
	{
	
		assert(isset($job_id));
	
		include('job_descriptions/pending_jobs/assign_host_department.inc');
	}
	else
	{
		include('misc/loguser.inc');
	}
}

elseif ($continue == 'save_host_department')
{
	// The traffic director has chosen the host department and would now like to save it.

	if ($userlevel == TRAFFICDIRECTOR)
	{
		assert(isset($job_id));
		
		if (trim($assigned_host_department) == '')
		{
			error("You must choose a department to be assigned as this job's new host department.");
			include('job_descriptions/pending_jobs/assign_host_department.inc');
		}
		else
		{
			include('job_descriptions/pending_jobs/save_host_department.inc');
			if ($userlevel == OFFICE)
		        {
		                include('job_descriptions/pending_jobs/pending_jobs_list.inc');
		        }
		        elseif ($userlevel == TRAFFICDIRECTOR)
		        {
		                include('job_descriptions/pending_jobs/traffic_director_job_list.inc');
		        }
		}
	}
}

elseif ($continue == 'Confirm Rejection')
{
	/*
	 The user has confirm their rejection of this company and contact.  Delete their 
	 information, as well as the job description they have posted.
	*/

	assert(isset($job_id));

	$pendingEmployer = new PendingEmployer('');
	$pendingEmployer->load_pending_values($job_id);

	include('job_descriptions/pending_jobs/reject_company_contact.inc');
}

elseif ($continue == 'newCompany_newContact')
{
	/*
	 The user has confirmed that they want to add the company and contact, even though
	 they have matched with other entries in the databae.
	*/

	assert(isset($job_id));

	$choice = 'newCompany_newContact';
	
	include('job_descriptions/pending_jobs/approve_job/method_for_adding.inc');
}

elseif ($continue == 'oldCompany_oldContact')
{
	// The company and contact are both existing ones in the database.

	assert(isset($job_id));

	include('job_descriptions/pending_jobs/oldCompany_oldContact.inc');
}

elseif ($continue == 'oldCompany_newContact')
{
	// The company is an existing one in the database, but the contact is a new one.

	assert(isset($job_id));

	include('job_descriptions/pending_jobs/oldCompany_newContact.inc');
}

elseif ($continue == 'newCompany_oldContact')
{
	// The company is a new one, but the contact is already in the database.

	assert(isset($job_id));

	include('job_descriptions/pending_jobs/newCompany_oldContact.inc');
}

elseif ($continue == 'method_for_adding')
{
	/*
	 They have chosen how they would like to go about adding the information into the database.
	 Load the file that will determine their choice and carry it out.
	*/

	assert(isset($job_id));
	
	if (!$choice)
	{
		error("You must choose one of the options below before you can proceed.");
		if ($situation == 'newCompany_oldContact')
		{
			include('job_descriptions/pending_jobs/newCompany_oldContact.inc');
		}
		elseif ($situation == 'oldCompany_oldContact')
		{
			include('job_descriptions/pending_jobs/oldCompany_oldContact.inc');
		}
		else
		{
			include('job_descriptions/pending_jobs/oldCompany_newContact.inc');
		}
	}
	elseif ($choice == 'Cancel')
	{
		if ($userlevel == OFFICE)
        	{
        	        include('job_descriptions/pending_jobs/pending_jobs_list.inc');
        	}
        	elseif ($userlevel == TRAFFICDIRECTOR)
        	{
        	        include('job_descriptions/pending_jobs/traffic_director_job_list.inc');
        	}
	}
	else
	{
		assert(isset($choice));
        // choice will be set to updateDepComp_addContact upon arriving here when choosing a company or division in the pop-up box.
		include('job_descriptions/pending_jobs/approve_job/method_for_adding.inc');
	}
}

elseif ($continue == 'Delete Incomplete Information')
{
	// They're done with this contact/company's incomplete information, so delete it from the database.

	assert(isset($contact_id));

	include('job_descriptions/pending_jobs/delete_incomplete_information.inc');

	notify("The contact (and company if applicable) has been successfully deleted.");

	if ($userlevel == OFFICE)
	{
		include('job_descriptions/pending_jobs/pending_jobs_list.inc');
	}
	elseif ($userlevel == TRAFFICDIRECTOR)
	{
		include('job_descriptions/pending_jobs/traffic_director_job_list.inc');
	}
}

?>
