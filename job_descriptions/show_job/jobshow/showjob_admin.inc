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
 | Filename: showjob_admin.inc                                                  |
 +------------------------------------------------------------------------------+
 | Description: This code takes the job_id that is passed to it and withdraws   |
 | all the information pertaining to the specific job out of the database, and  |
 | displays it.                                                                 |
 +------------------------------------------------------------------------------+

*/

include('job_descriptions/show_job/jobshow/PDFjob_admin.inc');
include_once('contact/contact_display_functions.inc');

$job_id = urldecode($job_id);

$job = new EditJob('');
$job->loadValues($job_id);

if ($job->salary_amount1 == '' XOR $job->salary_amount2 == '') 
{
	/*
	 This should never happen, but just in case it does...
	*/

	if ($job->salary_amount1 == '')
	{
		$job->salary_amount1 = $job->salary_amount2;
	}
	if ($job->salary_amount2 == '')
	{
		$job->salary_amount2 = $job->salary_amount1;
	}
}

if ($job->status != INT_ONLY && $job->status != REJECTED)
{


    $jobOutput .= ("
        <center>
        <table border='1' width='100%'>
        <tr>
        <td>
        <table border='0' cellpadding='4' cellspacing='0' width='100%'>
        ");

        $jobOutput .= ("<tr>");
            $jobOutput .= ("<td align='center' colspan='2'>");
            $jobOutput .= ("<h3>");
            $jobOutput .= ($job->job_code ? $job->job_code . ": " : "&nbsp;");
            $jobOutput .= ($job->position_title);
            $jobOutput .= ("<br /></h3>");
            $jobOutput .= ("</td>");
        $jobOutput .= ("</tr>");
        $jobOutput .= ("<tr>");	
            $jobOutput .= ("<td colspan='2'>");
            $jobOutput .= ("<table width='100%'>");
            $jobOutput .= ("<tr>");
            $jobOutput .= ("<td><b>Contact name:</b>");
            $jobOutput .= ("</td>");
            $jobOutput .= ("<td>" . ($job->contact_name ? $job->contact_name : "&nbsp;"));
            $jobOutput .= ("</td>");
            $jobOutput .= ("<td><b>Contact e-mail:</b>");
            $jobOutput .= ("</td>");
            $jobOutput .= ("<td>" . ($job->contact_email ? $job->contact_email : "&nbsp;"));
            $jobOutput .= ("</td>");
            $jobOutput .= ("</tr>");
            $jobOutput .= ("<tr>");
            $jobOutput .= ("<td colspan='4'>");
            $jobOutput .= ("&nbsp;");
            $jobOutput .= ("</td>");
            $jobOutput .= ("</tr>");
            $jobOutput .= ("<tr>");
            $jobOutput .= ("<td><b>Contact phone:</b>");
            $jobOutput .= ("</td>");
            $jobOutput .= ("<td>");
            if ($job->contact_phone == USE_COMPANY)
            {
                    $sql = ("
                            SELECT DISTINCT phone
                            FROM employer_company
                            WHERE employer_id='" . $job->employer_id . "'
                            ");
                    $result = $GLOBALS['dbh']->Execute($sql);
                    $row = $result->FetchRow();
            
                    $company_phone = $row["phone"];
            
                    $jobOutput .= ($company_phone . "&nbsp;&nbsp;<b class='black'>(Using Company Phone)</b>");
            }
            elseif ($job->contact_phone == USE_DEPARTMENT)
            {
                    $sql = ("
                            SELECT DISTINCT ed.phone
                            FROM employer_department AS ed, contact_employer AS ce
                            WHERE ce.contact_id='" . $job->contact_id . "'
                            AND ed.department_id=ce.department_id
                            ");
                    $result = $GLOBALS['dbh']->Execute($sql);
                    $row = $result->FetchRow();
            
                    $department_phone = $row["phone"];
            
                    if ($department_phone == USE_COMPANY)
                    {
                            $sql = ("
                                    SELECT DISTINCT phone
                                    FROM employer_company
                                    WHERE employer_id='" . $job->employer_id . "'
                                    ");
                            $result = $GLOBALS['dbh']->Execute($sql);
                            $row = $result->FetchRow();
            
                            $department_phone = $row["phone"];
                    }
            
                    $jobOutput .= ($department_phone . "&nbsp;&nbsp;<b class='black'>(Using Division Phone)</b>");
            }
            else
            {
                    $jobOutput .= (($job->contact_phone) ? $job->contact_phone : "&nbsp;");
            }
            $jobOutput .= ("</td>");
            $jobOutput .= ("<td><b>Contact fax:</b>");
            $jobOutput .= ("</td>");
            $jobOutput .= ("<td>");
                    
            if ($job->contact_fax == USE_COMPANY)
            {
                    $sql = ("
                            SELECT DISTINCT fax
                            FROM employer_company
                            WHERE employer_id='" . $job->employer_id . "'
                            ");
                    $result = $GLOBALS['dbh']->Execute($sql);
                    $row = $result->FetchRow();
            
                    $company_fax = $row["fax"];
            
                    $jobOutput .= ($company_fax . "&nbsp;&nbsp;<b class='black'>(Using Company Fax)</b>");
            }
            elseif ($job->contact_fax == USE_DEPARTMENT)
            {
                    $sql = ("
                            SELECT DISTINCT ed.fax
                            FROM employer_department AS ed, contact_employer AS ce
                            WHERE ce.contact_id='" . $job->contact_id . "'
                            AND ed.department_id=ce.department_id
                            ");
                    $result = $GLOBALS['dbh']->Execute($sql);
                    $row = $result->FetchRow();
            
                    $department_fax = $row["fax"];
            
                    if ($department_fax == USE_COMPANY)
                    {
                            $sql = ("
                                    SELECT DISTINCT fax
                                    FROM employer_company
                                    WHERE employer_id='" . $job->employer_id . "'
                                    ");
                            $result = $GLOBALS['dbh']->Execute($sql);
                            $row = $result->FetchRow();
            
                            $department_fax = $row["fax"];
                    }
            
                    $jobOutput .= ($department_fax . "&nbsp;&nbsp;<b class='black'>(Using Division Fax)</b>");
            }
            else
            {
                    $jobOutput .= (($job->contact_fax) ? $job->contact_fax : "&nbsp;");
            }

            $jobOutput .= ("</td>");
            $jobOutput .= ("</tr>");
            if($job->contact_cellphone)
            {  
                $jobOutput .= ("<tr>");
                $jobOutput .= ("<td colspan='4'>");
                $jobOutput .= ("&nbsp;");
                $jobOutput .= ("</td>");
                $jobOutput .= ("</tr>");
                $jobOutput .= ("<tr>");
                $jobOutput .= ("<td><b>Contact cellphone:</b>");
                $jobOutput .= ("</td>");
                $jobOutput .= ("<td>");
                $jobOutput .= ($job->contact_cellphone. "&nbsp;");
                $jobOutput .= ("</td>");
                $jobOutput .= ("</tr>");
            }    
            
            $jobOutput .= ("</table>");
            $jobOutput .= ("</td>");
        $jobOutput .= ("</tr>");
        


        // If they have specified that they would like
        // to receive online applications, then display	
        // the e-mail address that they will be sent to.

        $jobOutput .= ("<tr>");
            $jobOutput .= ("<td colspan='2'><br />");
            $jobOutput .= ("<table width='100%'>");
        
        if ($job->apply_method == JOB_APP_METHOD_ONLINE)
        {
        $jobOutput .= ("<tr>");
            $jobOutput .= ("<td colspan='2' align='center' nowrap='nowrap'><b>Send online applications to:</b>&nbsp;");
            $jobOutput .= ($job->receive_address); 
            $jobOutput .= ("</td>");
        $jobOutput .= ("</tr>");
        }

        if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR || $auth->department == DIRECTORS_OFFICE_DEPARTMENT)
        {
        $jobOutput .= ("<tr>");
            $jobOutput .= ("<td colspan='2' align='center'><br /><b>Current status of job:</b>&nbsp;");
            $jobOutput .= ("<b class='black'>");
            $jobOutput .= (getAdminStatusName($job->admin_status));
            $jobOutput .= ("</b></td>");
        $jobOutput .= ("</tr>");
        }

        if ($job->host_department)
        {
            $jobOutput .= ("
                <tr>
                <td align='center' colspan='2'><br /><b>Preferred host department:</b>&nbsp;
                " . getDepartmentName($job->host_department) . "</td>
                </tr>
            ");
        }
        if ($job->entered_by || $job->entered_by==0 || $job->entered_by==NULL)
        {
                $jobOutput .= ("
                    <tr>
                        <td align='right'>
                            <br />
                            <b>Entered By:</b> 
                ");
                if($job->entered_by)
                {
                     $sql = ("
                        SELECT CONCAT(first_name, ' ', last_name) AS entered_by_name, type
                        FROM contact
                        WHERE contact_id='" . $job->entered_by . "'
                    ");
                     
                    $result = $GLOBALS['dbh']->Execute($sql);
                    $row = $result->FetchRow();

                    if($row['type']=="internal")
                    {     
                        $jobOutput .= (($row['entered_by_name']? $row['entered_by_name'] : "Unknown")." &nbsp;&nbsp;&nbsp;&nbsp;");
                        $jobOutput .= ("</td>");
                    }    
                    else
                    {
                        if($row['type']=="employer" || $row['type']=="potential_employer")
                        {
                            $jobOutput .= ("Employer &nbsp;&nbsp;&nbsp;&nbsp;");
                            $jobOutput .= ("</td>");
                        }
                    }
                }
                else
                {
                    $jobOutput .= ("Unknown")."&nbsp;&nbsp;&nbsp;&nbsp;";
                    $jobOutput .= ("</td>");
                }
        }
        
        if ($job->date_entered || $job->date_entered==NULL)
        {
            $jobOutput .= ("<td >");
            $jobOutput .= ("<br /><b>Entered On:</b>&nbsp;");
            $jobOutput .= ($job->date_entered? $job->date_entered: "Unknown Date");
            $jobOutput .= ("</td>");
            $jobOutput .= ("</tr>");
        }
        
        if ($job->comments)
        {
        $jobOutput .= ("<tr>");
                    $jobOutput .= ("<td colspan='2' align='center'<br /><b>Additional comments:</b>");
                    $jobOutput .= ("</td>");
            $jobOutput .= ("</tr>");

            $jobOutput .= ("<tr>");
                    $jobOutput .= ("<td colspan='2' align='center'>");
                    $jobOutput .= nl2br($job->comments);
                    $jobOutput .= ("</td>");
            $jobOutput .= ("</tr>");
            }

        if ($job->admin_comments)
        {
        $jobOutput .= ("<tr>");
            $jobOutput .= ("<td colspan='2' align='center'><br /><br /><b>Admin comments<br />(only viewable by office staff):</b>");
            $jobOutput .= ("</td>");
        $jobOutput .= ("</tr>");
        $jobOutput .= ("<tr>");
            $jobOutput .= ("<td colspan='2' align='center'><br />" . $job->admin_comments);
            $jobOutput .= ("</td>");
        $jobOutput .= ("</tr>");
        }

        $jobOutput .= ("</table>");
            $jobOutput .= ("</td>");
            $jobOutput .= ("</tr>");
        $contact_action_string = showMostRecentContactActions($job->contact_id);
        if (strlen($contact_action_string)) {
            $jobOutput .= ("<tr><td colspan='2'><hr />");
            $jobOutput .= $contact_action_string;
            $jobOutput .= ("</td></tr>");
        }
        $jobOutput .= ("<tr><td colspan='2'><br /><hr /></td></tr>");
        $jobOutput .= ("<tr>");
            $jobOutput .= ("<td colspan='2' align='center'>");
            $jobOutput .= ("<b class='black'>Students will see the information below</b>");
            $jobOutput .= ("</td>");
        $jobOutput .= ("</tr>");
        $jobOutput .= ("<tr><td colspan='2'><hr /></td></tr>");

    //
    // Students will see all of the information below this point.
    //

        $jobOutput .= ("<tr>");
            $jobOutput .= ("<td width='50%' align='right'><b>Job code:</b>");
            $jobOutput .= ("</td>");

            if ($job->job_code != '')
            {
                $jobOutput .= ("<td width='50%' align='left'><b class='black'>" . ($job->job_code) . "</b>");
                $jobOutput .= ("</td>");
            } else
            {
                $jobOutput .= ("<td>");
                $jobOutput .= ("<b class='black'>Not yet set</b>");
                $jobOutput .= ("</td>");
            }

        $jobOutput .= ("</tr>");
        if ($job->employer_code)
        {
            $jobOutput .= ("<tr>");
                $jobOutput .= ("<td align='right'><b>Employer job code:</b>");
                $jobOutput .= ("</td>");
                $jobOutput .= ("<td>");
                $jobOutput .= ("<b class='black'>" . $job->employer_code . "</b>");
                $jobOutput .= ("</td>");
            $jobOutput .= ("</tr>");
        }
        $jobOutput .= ("<tr>");
            $jobOutput .= ("<td align='right'><b>Title of position:</b>");
            $jobOutput .= ("</td>");
            $jobOutput .= ("<td>" . ($job->position_title));
            $jobOutput .= ("</td>");
        $jobOutput .= ("</tr>");
        $jobOutput .= ("<tr>");
            $jobOutput .= ("<td align='right'><b>Location of job:</b>");
            $jobOutput .= ("</td>");
            $jobOutput .= ("<td>");
            $jobOutput .= ($job->city);

            if (trim($job->provstate))
            {
                $jobOutput .= (", " . getProvstateName($job->provstate));
            }

            $jobOutput .= (", " . getCountryName($job->country));
            $jobOutput .= ("</td>");
        $jobOutput .= ("</tr>");
        $jobOutput .= ("<tr>");
            $jobOutput .= ("<td align='right'><b>Number of positions:</b>");
            $jobOutput .= ("</td>");
            $jobOutput .= ("<td>" . $job->num_positions); 
            $jobOutput .= ("</td>");
        $jobOutput .= ("</tr>");
        $jobOutput .= ("<tr>");
            $jobOutput .= ("<td align='right'>");
            $jobOutput .= ("<b>Apply:</b>");
            $jobOutput .= ("</td>");
            $jobOutput .= ("<td>");

            switch( trim($job->apply_method))
            {
                case JOB_APP_METHOD_PAPER:
                    $jobOutput .= ("<b class='black'>Via paper to the dropboxes</b>");
                    break;
                case JOB_APP_METHOD_SPECIAL:
                    $jobOutput .= ("<b class='black'>Other - Special instructions.</b>");
                    break;
                case JOB_APP_METHOD_ONLINE: 
                default:
                    $jobOutput .= ("<b class='black'>Electronically</b>");
                    break;
            }

            $jobOutput .= ("</td>");
        $jobOutput .= ("</tr>");
        $jobOutput .= ("<tr>");
            $jobOutput .= ("<td align='right'><b>Term:</b>");
            $jobOutput .= ("</td>");
            $jobOutput .= ("<td>");
            $jobOutput .= (getMonth(getTermMonth($job->start_term)));
            $jobOutput .= (", " . $job->start_year);
            $jobOutput .= ("</td>");
        $jobOutput .= ("</tr>");
        $jobOutput .= ("<tr>");
            $jobOutput .= ("<td align='right'><b>Job closes at:</b>");
            $jobOutput .= ("</td>");
            $jobOutput .= ("<td><b class='black'>");

            if ($job->closing_date != '-1')
            {
                $jobOutput .= ($job->closing_time . ", ");
                $jobOutput .= (formatLongDate($job->closing_date));

                $jobOutput .= ("&nbsp;</b>");
                if ($job->closing_date < $current_date)
                {
                    $jobOutput .= ("<font style='color:red;'>closed</font><b class='black'>)</b>");
                }
            } else {
                $jobOutput .= ("<b class='black'>Not yet set</b>");
            }

            $jobOutput .= ("</td>");
        $jobOutput .= ("</tr>");
        $jobOutput .= ("<tr>");
            $jobOutput .= ("<td align='right'><b>Length of workterm:</b>");
            $jobOutput .= ("</td>");
            $jobOutput .= ("<td>");

            if ($job->start_date && $job->end_date)
            {
                $jobOutput .= ("Other - see start and end dates");
            }
            elseif (sizeof($job->workterm_lengths) == 1)
            {
                $jobOutput .= ($job->workterm_lengths[0] . " months");
            }
            else
            {
                for ($i = 0; $i < (sizeof($job->workterm_lengths) - 1); $i++)
                {
                    $jobOutput .= ($job->workterm_lengths[$i] . ", ");
                }
                $jobOutput .= ("or " . $job->workterm_lengths[$i] . " months");
            }

            $jobOutput .= ("</td>");
        $jobOutput .= ("</tr>");

        if ($job->start_date && $job->end_date)
        {
            $jobOutput .= ("<tr>");
                $jobOutput .= ("<td align='right'><b>Start date:</b>");
                $jobOutput .= ("</td>");
                $jobOutput .= ("<td>");
                $jobOutput .= (formatSpecialZeroDate($job->start_date));
                $jobOutput .= ("</td>");
            $jobOutput .= ("</tr>");

            $jobOutput .= ("<tr>");
                $jobOutput .= ("<td align='right'><b>End date:</b>");
                $jobOutput .= ("</td>");
                $jobOutput .= ("<td>");
                $jobOutput .= (formatSpecialZeroDate($job->end_date));
                $jobOutput .= ("</td>");
            $jobOutput .= ("</tr>");
        }

        if ($job->salary_amount1 != '')
        {
            $jobOutput .= ("<tr>");
                $jobOutput .= ("<td align='right'><b>Approximate<br />salary range:</b>");
                $jobOutput .= ("</td>");
                $jobOutput .= ("<td>");
                if ($job->salary_amount1 == $job->salary_amount2)
                {
                    $jobOutput .= ("$" . ($job->salary_amount1));
                }
                else
                {
                    $jobOutput .= ("$" . ($job->salary_amount1) . " to $" . ($job->salary_amount2));
                }
                $jobOutput .= (" per " . $job->salary_period);
                $jobOutput .= ("</td>");
            $jobOutput .= ("</tr>");
        }

        if ($job->workterm_hours != '')
        {
            $jobOutput .= ("<tr>");
                $jobOutput .= ("<td align='right'><b>Hours of work per week:</b>");
                $jobOutput .= ("</td>");
                $jobOutput .= ("<td>");
                $jobOutput .= ($job->workterm_hours);
                $jobOutput .= ("</td>");
            $jobOutput .= ("</tr>");
        }

        $jobOutput .= ("<tr>");
                $jobOutput .= ("<td nowrap='nowrap' align='right'>");
                $jobOutput .= ("<b>Receive applications from<br />students in these departments:</b>");
                $jobOutput .= ("</td>");
                $jobOutput .= ("<td>");
        
                for ($i = 0; $i < (sizeof($job->apply_departments) - 1); $i++)
                {
                        $jobOutput .= (getDepartmentName($job->apply_departments[$i]) . ", ");
                }
            $jobOutput .= (getDepartmentName($job->apply_departments[$i]));
        
                $jobOutput .= ("</td>");
        $jobOutput .= ("</tr>");

        if ($job->job_disciplines)
        {
                $jobOutput .= ("<tr>");
                        $jobOutput .= ("<td align='right'><b>Applicable disciplines within<br />the chosen departments:</b>");
                        $jobOutput .= ("</td>");
                        $jobOutput .= ("<td>");
                        $jobOutput .= ("<b class='black'>");
                $disc_codes = '';
                        foreach ($job->job_disciplines AS $jd)
                        {
                                $disc_codes .= getDisciplineCode($jd) . ", ";
                        }
        
                        $jobOutput .= substr($disc_codes, 0, -2);
                        $jobOutput .= ("</b>");
                        $jobOutput .= ("</td>");
                $jobOutput .= ("</tr>");
        }

        if ( trim($job->other_postings) != '')
        {
            $jobOutput .= ("<tr>");
                $jobOutput .= ("<td align='right'><b>Job is also posted at:</b>");
                $jobOutput .= ("</td>");
                $jobOutput .= ("<td>");
                $jobOutput .= ( trim($job->other_postings));
                $jobOutput .= ("</td>");
            $jobOutput .= ("</tr>");
        }

        $jobOutput .= ("<tr><td colspan='2'><br /></td></tr>");

        $jobOutput .= ("<tr>");
            $jobOutput .= ("<td align='right'><b>Minimum academic<br />year completed:</b>");
            $jobOutput .= ("</td>");
            $jobOutput .= ("<td>");
            $jobOutput .= ($job->min_academic . " year");
            $jobOutput .= (($job->min_academic == '1') ? "" : "s");
            $jobOutput .= ("</td>");
        $jobOutput .= ("</tr>");

        $jobOutput .= ("<tr>");
            $jobOutput .= ("<td align='right'><b>Minimum number of<br />workterms completed:</b>");
            $jobOutput .= ("</td>");
            $jobOutput .= ("<td>");
            $jobOutput .= ($job->min_workterms . " workterm");
            $jobOutput .= (($job->min_workterms == '1') ? "" : "s");
            $jobOutput .= ("</td>");
        $jobOutput .= ("</tr>"); 

        $jobOutput .= ("<tr><td colspan='2'><br /></td></tr>");

        $jobOutput .= ("<tr>");
            $jobOutput .= ("<td align='right' nowrap='nowrap'><b>Employer/Company name:</b>");
            $jobOutput .= ("</td>");
            $jobOutput .= ("<td>" . ($job->company_name)); 
            $jobOutput .= ("</td>");
        $jobOutput .= ("</tr>");

        if ($job->company_website != '')
        {
        $jobOutput .= ("<tr>");
            $jobOutput .= ("<td align='right'><b>Employer's website:</b>");
            $jobOutput .= ("</td>");
            $jobOutput .= ("<td>");
            $jobOutput .= ("<a class='blue' target='employer_website' href='http://" . ($job->company_website . "'>" . ($job->company_website) . "</a>"));
            $jobOutput .= ("</td>");
        $jobOutput .= ("</tr>");
        }

        if ($job->company_description != '')
        {
            $jobOutput .= ("<tr>");
                $jobOutput .= ("<td valign='top' align='right'>");
                $jobOutput .= ("<b>Description of <br />Employer/Company:</b>");
                $jobOutput .= ("</td>");
                $jobOutput .= ("<td><br />");

                $jobOutput .= nl2br($job->company_description);

                $jobOutput .= ("</td>");
            $jobOutput .= ("</tr>");
        }

        if ($job->department_id && $job->department_id != 'none')
            {
                    $jobOutput .= ("        <tr><td colspan='2'><br /></td></tr>");
                    $jobOutput .= ("<tr>");         
                            $jobOutput .= ("<td align='right'><b>Company Division Name:</b></td>");
                            $jobOutput .= ("<td>" . $job->department_name . "</td>");
                    $jobOutput .= ("</tr>");
                                                    
                    if ($job->department_website != '' && $job->department_website != $job->employer_website)
                    {                       
                            $jobOutput .= ("
                                    <tr>            
                                    <td align='right'><b>Division's website:</b>
                                    </td>
                                    <td>
                                    <a class='blue' target='department_website' href='http://" . stripSlashes($job->department_website) . "'>" . stripSlashes($job->department_website) . "</a>
                                    </td>
                                    </tr>
                                    ");
                    }
                    
                    if ($job->department_description != '' && $job->department_description != $job->company_description)
                    {       
                            $jobOutput .= ("
                                    <tr>
                                    <td valign='top' align='right'>
                                    <b>Description of <br />Company Division:</b>
                                    </td>
                                    <td><br />
                                    ");
                            $jobOutput .= nl2br($job->department_description);

                            $jobOutput .= ("
                                    </td>
                                    </tr>
                                    ");
                    }
            }

        if ($job->for_student_comments && ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR))
        {
            $jobOutput .= ("<tr>");
                $jobOutput .= ("<td colspan='2' align='center'><br /><br /><b class='red'>Job status comments:</b>");
                $jobOutput .= ("</td>");
            $jobOutput .= ("</tr>");
            $jobOutput .= ("<tr>");
                $jobOutput .= ("<td colspan='2'>");
                $jobOutput .= nl2br($job->for_student_comments);
                $jobOutput .= ("<br />");
                $jobOutput .= ("</td>");
            $jobOutput .= ("</tr>");
        }

        $jobOutput .= ("<tr><td colspan='2'><br /></td></tr>");

        $jobOutput .= ("<tr>");
            $jobOutput .= ("<td valign='top' colspan='2' align='center'><b>Job description:</b>");
            $jobOutput .= ("</td>");
        $jobOutput .= ("</tr>");

        $jobOutput .= ("<tr>");
            $jobOutput .= ("<td colspan='2'>");

            $jobOutput .= nl2br($job->job_description);

            $jobOutput .= ("<br />");
            $jobOutput .= ("<br />");
            $jobOutput .= ("</td>");
        $jobOutput .= ("</tr>");

        $jobOutput .= ("<tr>");
            $jobOutput .= ("<td colspan='2' valign='top' align='center'><b>Skills required:</b>");
            $jobOutput .= ("</td>");
        $jobOutput .= ("</tr>");

        $jobOutput .= ("<tr>");
            $jobOutput .= ("<td colspan='2'>");

            $jobOutput .= nl2br($job->skills_required);

            $jobOutput .= ("<br />");
            $jobOutput .= ("<br />");
            $jobOutput .= ("</td>");
        $jobOutput .= ("</tr>");

        if ($job->special_requirements != '')
        {
        $jobOutput .= ("<tr>");
            $jobOutput .= ("<td colspan='2' valign='top' align='center' nowrap='nowrap'><b>Special requirements:</b>");
            $jobOutput .= ("</td>");
        $jobOutput .= ("</tr>");
        
        $jobOutput .= ("<tr>");
            $jobOutput .= ("<td colspan='2'>");

            $jobOutput .= nl2br($job->special_requirements);

            $jobOutput .= ("<br />");
            $jobOutput .= ("<br />");
            $jobOutput .= ("</td>");
        $jobOutput .= ("</tr>");
        }

        if ((($job->special_instructions != '') || ($job->show_special_instr_button == '1')) && ($job->apply_method == JOB_APP_METHOD_SPECIAL))
        {
        $jobOutput .= ("<tr>");
            $jobOutput .= ("<td colspan='2' valign='top' align='center' nowrap='nowrap'><b>Special Instructions:</b>");
            $jobOutput .= ("</td>");
        $jobOutput .= ("</tr>");

        $jobOutput .= ("<tr>");
            $jobOutput .= ("<td colspan='2'>");

            $jobOutput .= nl2br($job->special_instructions);
            $jobOutput .= ("<br />");

            if ($job->show_special_instr_button == '1')
            {
            $jobOutput .= ("Using special instructions button: Yes <br />");
            $jobOutput .= ("Special instructions button text: \"" . ($job->special_instr_button) . "\"");
            }

            $jobOutput .= ("</td>");
        $jobOutput .= ("</tr>");
        }

        if ($job->closing_date == '-1')
        {
            $jobEnd1 .= ("<tr>");
                $jobEnd1 .= ("<td colspan='2'>");
                $jobEnd1 .= ("<br /><hr />");
                $jobEnd1 .= ("</td>");
            $jobEnd1 .= ("</tr>");
            $jobEnd1 .= ("<tr>");
                $jobEnd1 .= ("<td colspan='2' align='center'>");

                if ($job->closing_date == '-1')
                {
                    $jobEnd1 .= ("<b>Closing Date </b>");
                }

                $jobEnd1 .= ("<b class='black'>will need to be set before this job can be posted.</b>");
                $jobEnd1 .= ("</td>");
            $jobEnd1 .= ("</tr>");
            $jobEnd1 .= ("<tr>");
                $jobEnd1 .= ("<td colspan='2'>");
                $jobEnd1 .= ("<hr />");
                $jobEnd1 .= ("</td>");
            $jobEnd1 .= ("</tr>");
        } 
        else if ($job->status == CANCELLED)
        {
            $jobEnd1 .= ("<tr>");
                $jobEnd1 .= ("<td colspan='2'>");
                $jobEnd1 .= ("<br /><hr />");
                $jobEnd1 .= ("</td>");
            $jobEnd1 .= ("</tr>");
            $jobEnd1 .= ("<tr>");
                $jobEnd1 .= ("<td colspan='2' align='center'>");
                $jobEnd1 .= ("This job has been <b>CANCELLED</b>.  The status will need to be changed to <b class='black'>POSTED</b> before students will be able to see the job again.");
                $jobEnd1 .= ("</td>");
            $jobEnd1 .= ("</tr>");
            $jobEnd1 .= ("<tr>");
                $jobEnd1 .= ("<td colspan='2'>");
                $jobEnd1 .= ("<hr />");
                $jobEnd1 .= ("</td>");
            $jobEnd1 .= ("</tr>");
        } 
        else if ($job->status == CHECKED || $job->status == NOT_CHECKED)
        {
            $jobEnd1 .= ("<tr>");
                $jobEnd1 .= ("<td colspan='2'>");
                $jobEnd1 .= ("<br /><hr />");
                $jobEnd1 .= ("</td>");
            $jobEnd1 .= ("</tr>");
            $jobEnd1 .= ("<tr>");
                $jobEnd1 .= ("<td colspan='2' align='center'>");
                $jobEnd1 .= ("This job's <b>status</b> will need to be updated to <b class='black'> POSTED </b> before students can see this job.");
                $jobEnd1 .= ("</td>");
            $jobEnd1 .= ("</tr>");
            $jobEnd1 .= ("<tr>");
                $jobEnd1 .= ("<td colspan='2'>");
                $jobEnd1 .= ("<hr />");
                $jobEnd1 .= ("</td>");
            $jobEnd1 .= ("</tr>");
        } 
        elseif ($job->status == CLOSED)
        {
            $jobEnd1 .= ("<tr>");
                $jobEnd1 .= ("<td colspan='2'>");
                $jobEnd1 .= ("<br /><hr />");
                $jobEnd1 .= ("</td>");
            $jobEnd1 .= ("</tr>");
                $jobEnd1 .= ("<td colspan='2' align='center'>");
                $jobEnd1 .= ("This job has <b>CLOSED</b>.  It will need to be <b class='black'>REPOSTED</b> before students will be able to apply to the job again.");
                $jobEnd1 .= ("</td>");
            $jobEnd1 .= ("</tr>");
            $jobEnd1 .= ("<tr>");
                $jobEnd1 .= ("<td colspan='2'>");
                $jobEnd1 .= ("<hr />");
                $jobEnd1 .= ("</td>");
            $jobEnd1 .= ("</tr>");
        }


        $formEnd .= ("<tr>");
            $department_ids = $departments_in_group;
            if($job->status == CLOSED || $job->status == CANCELLED)
            {
                if (in_array($job->host_department, $department_ids) || $userlevel == TRAFFICDIRECTOR)
                {
                    $formEnd .= ("<td valign='middle' align='right'>");
                }
                else
                {
                    $formEnd .= ("<td valign='middle' align='right'>");
                }
            }
            else
            {
                $formEnd .= ("<td colspan='2' valign='middle' align='center'>");
            }
            $formEnd .= ("
                <form method='post' action='" . ($PHP_SELF) . "'>
                <input type='hidden' name='select' value='edit_job' />
                <input type='hidden' name='job' value='" . ( isset($job) ? packObject(($job)) : "") . "' />
                <input type='hidden' name='job_id' value='" . ($job_id) . "' />
                ");
            
            /*
             If the user is part of a department whose group can edit this job,
             display the button allowing them to do so.
            */


            if (in_array($job->host_department, $department_ids) || $userlevel == TRAFFICDIRECTOR)
            {
                $formEnd .= ("<input type='submit' value='Edit/Update status' />");
                $formEnd .= ("&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;");
            }

            $formEnd .= ("<input type='hidden' name='HTMLjob' value='" . urlencode($PDFOutput) . "' />");
            $formEnd .= ("<input type='submit' name='PDF' value='View in PDF' />");
            $formEnd .= ("<br /><br />");
            $formEnd .= ("</form>");
            $formEnd .= ("</td>");
            
            if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
            {
                if(($job->status == CLOSED || $job->status == CANCELLED) 
                    && ($job->status != INT_ONLY || $job->status != POSTED || $job->status != REJECTED ))
                {
                    $formEnd .= ("<td valign='middle' align='left'>");
                    $formEnd .= ("<form method='post' action='" . ($PHP_SELF) . "'>");
                    $formEnd .= ("<input type='hidden' name='select' value='add_job' />");
                    $formEnd .= ("<input type='hidden' name='continue' value='template_job_chosen' />");
                    $formEnd .= ("<input type='hidden' name='job_id' value='" . ($job_id) . "' />");
                    $formEnd .= ("<input type='hidden' name='contact_id' value='" . ($job->contact_id) . "' />");
                    $formEnd .= ("<input type='hidden' name='employer_id' value='" . ($job->employer_id) . "' />");
                    $formEnd .= ("<input type='hidden' name='department_id' value='" . ($job->department_id) . "' />");
                    $formEnd .= ("<input type='submit' value='Repost this job' />");
                    $formEnd .= ("<br /><br />");
                    $formEnd .= ("</form>");
                    $formEnd .= ("</td>");
                }
            }
            $formEnd .= ("</tr>");

        $jobEnd2 .= ("</table>");
        $jobEnd2 .= ("</td>");
        $jobEnd2 .= ("</tr>");
        $jobEnd2 .= ("</table>");
        $jobEnd2 .= ("</center>");

    echo($jobOutput . $jobEnd1 . $formEnd . $jobEnd2);
} 
else
{
    error("Job ".$job->job_code." has no job description as this job is an interview only job, or the job was rejected.");
}

?>

