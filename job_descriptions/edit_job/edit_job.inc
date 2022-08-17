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
 | Filename: edit_job.inc                                                       |
 +------------------------------------------------------------------------------+
 | Description: determines whether or not to display the edit job form,         |
 | cancel the job or update the job                                             |
 +------------------------------------------------------------------------------+

*/

include('job_descriptions/edit_job/EditJob.class');

if (is_string($job)){
    $job = unpackObject($job);
}

$result = $GLOBALS['dbh']->Execute("select CURRENT_DATE");
$row = $result->FetchRow();
$current_date = $row["CURRENT_DATE"];

if (!($job_id))
{
	echo "</td><td align='left'><img src='".$GLOBALS['colorscheme']['topbar_right']."' alt='' /></td></tr></table>";

	// if a job hasn't been selected to edit, show the list of jobs available
	echo("<table border='0' cellspacing='0' cellpadding='2' width='99%'><tr><td>");
	if ($userlevel == OFFICE)
	{
		echo("<h3 align='left'>Job Descriptions - Edit</h3>");
		include('job_descriptions/edit_job/admin_edit_list.inc'); 
	}
	elseif ($userlevel == TRAFFICDIRECTOR)
	{
		echo("<h3 align='left'>Job Descriptions - Edit</h3>");
		include('job_descriptions/edit_job/trafficdirector_edit_list.inc');
	}
	elseif ($userlevel == EMPLOYER || $userlevel == HREMPLOYER || $userlevel == ADMINEMPLOYER)
	{
		include('job_descriptions/edit_job/employer_edit_list.inc');
	}
	else
	{
		include('misc/loguser.inc');
	}
}
else
{

	/*
	 We need to put in some sort of security check here to make sure that employers aren't trying to edit
	 a job that isn't theirs.
	*/

	assert($job_id);

	if (isJobInfoEditable($job_id))
	{

		$URL = "$PHP_SELF&amp;select=edit_job&amp;job_id=$job_id";
			
        	// ...show the job form
	
		if ($submit == 'Save Changes' || $submit == 'confirm_no_interview' || $continue == 'Save Changes')
		{	
			$sql = ("
				SELECT DISTINCT job_code
				FROM job_info
				WHERE job_id='" . $job_id . "'
				");
			$result = $GLOBALS['dbh']->Execute($sql);
			$row = $result->FetchRow();
			$job->job_code = $row["job_code"];
		} else
		{
			// get the data from the database
			$job = new EditJob('');
			$job->loadValues($job_id);
			if ($order)
			{
				/*
				 If we were brought here by an order href, we don't want to include
				 the division and contact values for this job, as the user needs
				 to choose them from the newly available ones, unless the companies
				 we're showing start with the same letter as the jobs current company
				 does.
				*/
	
				if (!(preg_match("/\b" . $order . ".*/i", trim($job->company_name))))
				{
					$job->employer_id = '';
					$job->department_id = '';
					$job->contact_id = '';
				}
			}
		}
	 	include('job_descriptions/edit_job/edit_header.inc');	

		echo("<table border='0' cellspacing='0' cellpadding='0' width='97%'><tr><td>");
		//echo("<h3 align='left'>Job Descriptions - Edit ". $job->job_code . "</h3>");

		switch ($level1)
	        {
	                case "contact":
	                        if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
	                        {
	                                include('job_descriptions/edit_job/office/edit_contact.inc');
	                        }
	                        elseif ($userlevel == EMPLOYER || $userlevel == HREMPLOYER)
	                        {
	                                include('job_descriptions/edit_job/employer/edit_contact.inc');
	                        }
	                        break;
			case "host_department":
				if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
				{
					include('job_descriptions/edit_job/edit_host_department.inc');
				}
				else
				{
					include('misc/loguser.inc');
				}
				break;
	                case "jobinfo":
	                default:
	                        include('job_descriptions/edit_job/edit_job_info.inc' );
	                        break;
	        }	
	}
	else
	{
		echo ("</td><td align='left'><img src='".$GLOBALS['colorscheme']['topbar_right']."' alt='' /></td></tr></table>");
		echo("<table border='0' cellspacing='0' cellpadding='2' width='99%'>");
		echo("<tr>");
			echo("<td>");
			echo("<h3 align='left'>Job Descriptions - Edit</h3>");
			if ($userlevel == ADMINISTRATOR || $userlevel == OFFICE)
			{
				$msg = ("You have attempted to edit a job that is either not associated with your department");
				$msg .= (" or is in the past.  Please choose a job from those displayed below.");
				error($msg);
				include('job_descriptions/edit_job/admin_edit_list.inc');
			}
			elseif ($userlevel == TRAFFICDIRECTOR)
			{
				$msg = ("You have attempted to edit a job that is in the past.  Please choose a job from those displayed below.");
				error($msg);
				include('job_descriptions/edit_job/trafficdirector_edit_list.inc');
			}
			elseif ($userlevel == EMPLOYER || $userlevel == HREMPLOYER)
			{
				$msg = ("You have attempted to edit a job that is either not registered with your company or is in the past.");
				$msg .= ("  Please choose a job from those displayed below.");
				error($msg);
				include('job_descriptions/edit_job/employer_edit_list.inc');
			}
	}
				 
}     

echo("</td>");
echo("</tr>");
echo("</table>");

?>
