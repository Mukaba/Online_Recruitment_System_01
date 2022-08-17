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
 | Filename: applicant_list.inc                                                 |
 +------------------------------------------------------------------------------+
 | Description: This file is used to pull all of the applicants out of the      |
 | database for a given job.                                                    |
 +------------------------------------------------------------------------------+

*/

assert(isset($job_id));

$sql = "SELECT DISTINCT ji.job_code, ji.apply_method, ec.company_name, ji.status, ji.receive_address as job_email,
        CONCAT(c.first_name, ' ', c.last_name) as contact_name, c.email as contact_email, el.login_id, 
        ji.term_id, ji.year, d.department_name, ji.employer_code
        FROM employer_company as ec, job_info as ji
        LEFT JOIN contact as c ON c.contact_id = ji.contact_id
        LEFT JOIN employer_login as el ON el.contact_id = ji.contact_id
        LEFT JOIN department AS d ON d.department_id = ji.department_id
        WHERE ji.job_id='" . $job_id . "' AND ec.employer_id=ji.employer_id";
$result = $GLOBALS['dbh']->Execute($sql);
$row = $result->FetchRow();

$job_term_id        = $row["term_id"];
$job_year           = $row["year"];
$job_code           = $row["job_code"];
$comp_name          = $row["company_name"];
$apply_method       = $row["apply_method"];
$job_status         = $row["status"];
$job_email          = $row["job_email"];
$contact_name       = $row["contact_name"];
$contact_email      = $row["contact_email"];
$contact_has_login  = $row["login_id"];
$host_department    = $row["department_name"];
$employer_code      = $row["employer_code"];

/*
 Pull out the applicants for this job.  For now, ignore that they may be with another department.
 If this module is to be used in the future, THIS MUST BE CHANGED.
*/
$sql = "SELECT DISTINCT a.application_id, a.resume_id, a.coverletter_id, a.transcript, 
        CONCAT(s.first_name, ' ', s.last_name) AS name, s.student_number, a.app_status
        FROM applications AS a, student AS s
        WHERE a.job_id='$job_id'
        AND s.student_number=a.student_number
        ORDER BY s.last_name";
$result = $GLOBALS['dbh']->Execute($sql);

echo ("<form method='post' action='$PHP_SELF'>");
echo ("<input type='hidden' name='PDF' value='1' />");
echo ("<input type='hidden' name='come_from_apps' value='1' />");
echo ("<input type='hidden' name='select' value='applications' />");
echo ("<input type='hidden' name='option' value='$option' />");
echo ("<input type='hidden' name='job_id' value='$job_id' />");
echo ("<input type='hidden' name='job_code' value='$job_code' />");
echo ("<input type='hidden' name='option' value='$option' />");
echo ("<input type='hidden' name='apply_method' value='$apply_method' />");

// after an application has been added, they are brought here if they chose not to "Add More"
// this simply notifies them of the appliations they added
// NOTE: these also appear in add_application.inc
if ($successful) {

	notify ("Applications for the following students have been added successfully: <br />$successful");
}

if ($unsuccessful) {

    error ("Errors were encountered when adding applications for the following students:<br />$unsuccessful");
}

// tried viewing selected or single applications, but none were selected
if ($none_selected) {

	notify ("No applications where selected. You much select at least one application.");
}

//print("<pre>");
//print_r($_COOKIE);
//print("</pre>");
//print("<pre>");
//print_r($_SESSION);
//print("</pre>");

// title

/* :EXPIRY: June 31 2005 - Patrick - Removal ov view/send pages
echo ("<h3>Job Applications - ". ($option == 'view' ? "View" : "Send"). "</h3>");
*/

// Display the quick menu... checking to see what should be put into it
echo ("<table width='99%' border='0' cellpadding='2' cellspacing='0'>");
echo ("<tr>");
    
    echo ("<td align='left'>");

echo ("<h3>Job Applications</h3>");
echo("</td><td align='right'>");
echo ("<select name='continue'>");
/* :EXPIRY: June 31 2005 - Patrick - Removal ov view/send pages
    if ($option == 'send') {
		echo ("<option value='jump_to_view'>Jump to View</option>");
	}
	else {
		echo ("<option value='jump_to_send'>Jump to Send</option>");
	}
    */
	echo ("<option value='E-mail All'>E-mail All Students</option>");
    echo ("<option value='E-mail Selected'>E-mail Selected Students</option>");
	if ($apply_method != JOB_APP_METHOD_SPECIAL) {

		if (($contact_email || ($contact_email != "") || ($contact_email != NULL)) || ($job_email || ($job_email != "") || ($job_email != NULL))) {
			echo ("<option value='E-mail Employer'>E-mail Employer</option>");
        }
		else {
			echo ("<option value='view_applicants'>No Employer E-mail</option>");
	    }

        if ($apply_method == JOB_APP_METHOD_ONLINE) {
			echo ("<option value='Employer Login'>Create/Resend Employer Login ID</option>");
	    }
    }
	echo ("<option value='create_pdf_list'>Create PDF List (printable)</option>");
    echo ("</select>");
    echo ("&nbsp; <input type='image' src='misc/images/arrowg.gif' alt='go' /> &nbsp; &nbsp;");
    echo ("</td>");
echo ("</tr>");
echo ("</table>");


// Display some general info about the job and the company it's associated with
echo ("<table>");
echo ("<tr>");
    echo ("<td align='left'><b>Company Name:</b></td>");
    echo ("<td>$comp_name</td>");
echo ("</tr>");

echo ("<tr>");
    echo ("<td align='left'><b>Job Code:</b></td>");
    echo ("<td><a href='$PHP_SELF&amp;select=view_job&amp;level1=job_id&amp;show_quick=true&amp;job_id=$job_id' target='_blank'>$job_code</a></td>");
echo ("</tr>");

echo ("<tr>");
    echo ("<td align='left'><b>Job Contact:</b></td>");
    echo ("<td>$contact_name ");
    if ($apply_method == JOB_APP_METHOD_ONLINE) {
        echo (($contact_has_login == NULL) ? "(Does <b class='black'>NOT</b> have login ID.)" : "(Has login ID.)");
	}
    echo ("</td>");
echo ("</tr>");

if ($employer_code) {

    echo ("<tr>");
        echo ("<td align='left'><b>Employer Job Code:</b></td>");
        echo ("<td>$employer_code</td>");
    echo ("</tr>");
}

echo ("<tr>");
    echo ("<td align='left'><b>Total Applicants:</b></td>");
    echo ("<td>". $result->RecordCount(). "</td>");
echo ("</tr>");
	
echo ("<tr>");
    echo ("<td align='left'><b>Host Department:</b></td>");
    echo ("<td>$host_department</td>");
echo ("</tr>");
echo ("</table>");


if ($job_status == CLOSED) {

	instruct("Please note, this job is now closed and the associated employer may be viewing submitted student application
	packages.  Students can no longer apply to the position unless added to the Applicants List by an administrator. Once a job
	closes and applications become employer viewable, you will not be able to delete any existing student from a job's Applicants
	List; attempting to remove a student from an Applicants List after its job closes will only change a student's status to
	'Unavailable', and the employer will not be able to view the 'Unavailable' student's application package.");
}


if ($result->RecordCount()) {
	
	echo ("<table cellspacing='0' cellpadding='0' border='1'>");
	echo ("<tr>");
		echo ("<td>");

	    echo ("<table border='0' cellpadding='2'>");
	    echo ("<tr>");
	        echo ("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Name</b>&nbsp;</td>");
	        echo ("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Student Number</b>&nbsp;</td>");
		    echo ("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Department(s)</b>&nbsp;</td>");
            //if ($apply_method == "online") {
            //  echo ("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Transcript Included</b>&nbsp;</td>");
            //}
			echo ("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Status</b>&nbsp;</td>");
			if ($apply_method != JOB_APP_METHOD_SPECIAL) {

				echo ("<td class='rowgrey' align='center'>&nbsp;<b class='white'>Eligible</b>&nbsp;</td>");
				echo ("<td class='rowgrey' align='center'><b class='white'>".
					(($apply_method == JOB_APP_METHOD_ONLINE) ? "Employer Viewable" : "Sent to Employer") ."</b></td>");
			}
			echo ("<td class='rowgrey' align='center'>&nbsp;&nbsp;&nbsp;</td>");
		echo ("</tr>");


    // reset it and intiate before using it
    $all_apps = array ();
    $rowd = 0;
    while ($row = $result->FetchRow()) {

        $rowclass = (($rowd % 2) ? "row0d" : "row1d" );
        $name = $row["name"];
        $transcript = $row["transcript"];
        $student_number = $row["student_number"];
        $app_status = $row["app_status"];
        $app_id = $row["application_id"];
        $status = "&nbsp;";

        $depts = getStudentDepartments ($student_number);
        $department = "";
        for ($i = 0; $i < sizeof ($depts); $i ++) {

            $department .= getDepartmentName ($depts[$i]) ."; ";
        }
        $department = substr ($department, 0, -2);

        // check to see if eligible
        $eligibleSQL = $GLOBALS['dbh']->Execute ("select ep.student_flags_id 
                from eligible_placed as ep, student_department as sd
                where sd.student_number='$student_number' AND sd.record_id=ep.record_id
                AND ep.term_id='$job_term_id' AND ep.year='$job_year'");

        if ($eligibleSQL->RecordCount()) {
            while ($isEligible = $eligibleSQL->FetchRow())
            {
                $eligible = $isEligible["student_flags_id"];
                // if we're eligible, then display it, otherwise continue to look for a placed flag. 
                if ($eligible == ELIGIBLE_FLAG)
                {
                    break;
                }
            }
        }
        else {
            $eligible = 0;
        }

        // quick check to see if certain buttons should go in
        switch ($app_status) {

            case APP_APPLIED_ONLINE: 
                $needActivate = true;
                $needRemove = true;
                $status = "Applied Online";
                break;

            case APP_ACTIVATED_ONLINE: 
                $needDeactivate = true;
                $needRemove = true;
                $status = "Activated Online";
                break;

            case APP_ADMIN_DELETED:	
                $needReplace = true;
                $status = "Admin Deleted";
                break;

            case APP_STUDENT_DELETED: 
                $needReplace = true;
                $status = "Student Deleted";
                break;

            case APP_UNAVAILABLE: 
                $needReplace = true;
                $status = "Unavailable";
                break;

            case APP_PAPER_APPLIED: 
                $needActivate = true;
                $needRemove = true;
                $status = "Applied Paper";
                break;

            case APP_PAPER_SENT: 
                $needDeactivate = true;
                $needRemove = true;
                $status = "Sent Paper";
                break;

            case APP_SPECIAL: 
                $status = "Applied/Activated Special";
                $needRemove = true;
                break;
        }

        $href = $PHP_SELF . "&amp;select=applications&amp;continue=view_applicants&amp;job_id=" . urlencode($job_id);
        $href .= ("&amp;job_code=" . urlencode($job_code) . "&amp;status=" . urlencode($status) . "&amp;PDF=1");
        $href2 = ($href . "&amp;view_selected_apps=1&amp;selected_app_id=$app_id&amp;student=" . urlencode($row["student_number"]));

        echo ("<tr>");
            echo ("<td align='left' class='$rowclass'>");
            echo ("&nbsp;". (($apply_method == JOB_APP_METHOD_ONLINE) ? "<a class='blue' href=\"$href2\">$name</a>" : "$name") ."&nbsp;");
            echo ("</td>\n");
            
            echo ("<td align='center' class='$rowclass'>");
            echo ("&nbsp;". (($apply_method == JOB_APP_METHOD_ONLINE) ? "<a class='blue' href=\"$href2\">$student_number</a>" : "$student_number") ."&nbsp;");
            echo ("</td>\n");

            echo ("<td align='center' class='$rowclass'>");
            echo ("&nbsp;$department&nbsp;");
            echo ("</td>");

            /*
            if ($apply_method == "online") {
                echo ("<td align='center' class='$rowclass'>");
                echo ("&nbsp;". ($transcript == 1 ? "Yes" : "No") ."&nbsp;");
                echo ("</td>");
            }
            */

            echo ("<td align='center' class='$rowclass'>");
            echo ("&nbsp;$status&nbsp;");
            echo ("</td>");

            if ($apply_method != JOB_APP_METHOD_SPECIAL) {

                echo ("<td align='center' class='$rowclass'>");
                echo ((($eligible == ELIGIBLE_FLAG) ? "Yes" : (($eligible == PLACED_FLAG) ? "Placed" : "<b class='red'>No</b>" )));
                echo ("</td>");

                echo ("<td align='center' class='$rowclass'>");
                echo ((($app_status == APP_ACTIVATED_ONLINE) ? "Yes" : (($app_status == APP_PAPER_SENT) ? "Yes" : "No" )));
                echo ("</td>");
            }
            echo ("<td align='center' class='$rowclass'>");
            echo ("<input type='checkbox' name='selected_app_id[]' value='$app_id' />");
            echo ("</td>\n");
        echo ("</tr>");

        // this will be used for viewing all applications
        // don't change name is used in apply.inc
        $all_apps[] = $app_id;
        $rowd ++;
    }
	
	echo ("</table>");
	echo ("</td>");
    echo ("</tr>");
	echo ("</table>");
	
	// pass array created above
	echo ("<input type='hidden' name='all_apps' value='". packObject(($all_apps)) ."' />");
	echo ("<br />");
}

else {

	notify("There are currently no applicants for this job.");
	echo ("<br />");
}


// Make the button strings
$application_buttons = "<input type='submit' name='continue' value='Add an Application' />&nbsp;";
if ($needRemove)  $application_buttons .= "<input type='submit' name='continue' value='Remove Applications' />&nbsp;";
if ($needReplace) $application_buttons .= "<input type='submit' name='continue' value='Replace Applications' />";
$application_buttons .= "<br /><br />";	

$view_application_buttons  = "<input type='submit' name='view_all_apps' value='View All Applications' />&nbsp; ";
$view_application_buttons .= "<input type='submit' name='view_selected_apps' value='View Selected Applications' />";
$view_application_buttons .= "<br /><br />";

if ($needActivate) $activate_buttons = "<input type='submit' name='continue' value='Activate Selected Applications' />&nbsp;";
if ($needDeactivate) $activate_buttons .= "<input type='submit' name='continue' value='Deactivate Applications' />&nbsp;";
if ($needActivate) $activate_buttons .= "<input type='submit' name='continue' value='Activate All Applications' />";
if ($needActivate || $needDeactivate) $activate_buttons.= "<br /><br />";
 

// Print the table
echo ("<table border='0' cellpadding='5' cellspacing='0'>");
echo ("<tr class='Row1'>");
echo ("<td class='row1' align='left'>");


print $application_buttons;

if ($result->RecordCount()){
    if($apply_method == JOB_APP_METHOD_ONLINE){
        echo $view_application_buttons;
        echo $activate_buttons;
    }



    $sql_interview = ("
            SELECT job_id 
            FROM interview
            WHERE job_id='$job_id'
            ");
    $result_interview=$GLOBALS['dbh']->Execute($sql_interview);
    if ($result_interview->RecordCount()==0)
    {
        echo("<input type='submit' name='continue' value='Shortlist All Students' />");
        echo(" &nbsp; ");
        echo("<input type='submit' name='continue' value='Shortlist Selected Students' />");
    } else {
        echo("An interview exists for this job");
    }
}
echo ("</td>");
echo ("</tr>");
echo ("</table>");

echo ("</form>");

?>
