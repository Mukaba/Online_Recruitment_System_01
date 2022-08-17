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
 | Filename: search.inc                                                         |
 +------------------------------------------------------------------------------+
 | Description: If the user has selected a job from the search results, show    |
 | him the details of that job.  Otherwise, show him the appropriate search     |
 | page.                                                                        |
 +------------------------------------------------------------------------------+

*/

?>
<!-- Close Header -->
</td><td align="left"><img src="<?php echo $GLOBALS['colorscheme']['topbar_right']; ?>" alt='' /></td></tr></table>
<?php

	include('job_descriptions/search_job/SearchJob.class');
	include('job_descriptions/search_job/searchJob_functions.inc');

	echo("<table border='0' cellspacing='0' cellpadding='2' width='99%'><tr><td>");
	echo("<h3 align='left'>Job Descriptions - Search</h3></td>");
    if($show_quick)
    {
        if( isset($quick_type) )
        {
            echo("<td align='right'>");
            switch($quick_type)
            {
                case "company_quickview":
                    include_once("contact/company_quickmenu.inc");
                    break;

                case "division_quickview":
                    include_once("contact/division_quickmenu.inc");
                    break;

                case "contact_quickview":
                    include_once("contact/contact_quickmenu.inc");
                    break;

                default:
                    assert(false);
            }
            echo("</td>");
        }
        else
        {
            include_once("job_descriptions/job_quickmenu.inc");
        }
    }	
   	echo("</tr>");	
    echo("<tr><td colspan='2'>");
	
	if ($job_id)
	{
		include('job_descriptions/edit_job/EditJob.class');
		
        if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
		{
			include('job_descriptions/show_job/jobshow/showjob_admin.inc');
		} 
        elseif ($userlevel == STUDENT || $userlevel == ADMINSTUDENT)
		{
			include('job_descriptions/show_job/jobshow/showjob_student.inc');
		} 
        elseif ($userlevel == HREMPLOYER || $userlevel == EMPLOYER)
		{
			include('job_descriptions/show_job/jobshow/showjob_employer.inc');
		} 
        else
		{
			include('misc/loguser.inc');
		}
	} 
	elseif ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
    {    
        include("job_descriptions/search_job/admin/search_admin.inc");
	}
	elseif ($userlevel == HREMPLOYER || $userlevel == EMPLOYER)
	{
		include("job_descriptions/search_job/employer/search_employer.inc");
	}
	elseif ($userlevel == STUDENT || $userlevel == ADMINSTUDENT)
	{
		include("job_descriptions/search_job/student/search_student.inc");
	}
	else
	{
		include("misc/loguser.inc");
	}
   	echo("</td></tr></table>");	
?>
