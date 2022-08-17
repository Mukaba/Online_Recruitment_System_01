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
 | Filename: add.inc                                                            |
 +------------------------------------------------------------------------------+
 | Description: The main file for adding applications, from here various        |
 | other files are called based on the progress of the application thus far.    |
 +------------------------------------------------------------------------------+

*/

assert(isset($job_id));
assert(isset($student_number));
$application_id = "";

// title at the top - get the job_code and position_title
$job_result = $GLOBALS['dbh']->Execute ("Select job_code, position_title from job_info where job_id='$job_id'");
$job_row = $job_result->FetchRow();

echo("<h3 align='left'>New Application for ".$job_row["job_code"].", ". $job_row["position_title"]."</h3>");


// since the student can hit the back button and try applying twice, I put this up here
//        Check and see if the student has already applied for this job.
$result = $GLOBALS['dbh']->Execute ("SELECT DISTINCT student_number, job_id
		FROM applications
		WHERE student_number='" . $student_number . "'
		AND job_id='" . $job_id . "'");

if ($result->RecordCount())
{
	error("You have already applied for this job.  No more than one application is needed for a job.");

}


// editing or viewing the coverletter to be attached
elseif ($next_step_edit_letter || $next_step_view_app || (false && !$next_step_insert)) 
{
	preg_match ("/(\d)_(\d)/", $coverletter_type, $matches);
	$coverletter_type = $matches[1];

	// need the name of the resume
	$resume = $GLOBALS['dbh']->Execute ("select name from resume where resume_id='$resume_id' AND student_number='$student_number';");
	$getResname = $resume->FetchRow();

	$resume_name = $getResname["name"];


	// check if it's generic or template, if yes, we need to get it 
	// only a choice of 0, 2, or 3 for coverletter_type, 0 is empty, the others are template/generic
	if ($coverletter_type != 0) {

		$generic = $GLOBALS['dbh']->Execute ("Select letter from coverletter where student_number='$student_number' 
					AND type_id='$coverletter_type'");
		$Getletter = $generic->FetchRow();

		$app_letter = $Getletter["letter"];

	}


	// since there might have been a generic, that's why these two are together
	if (($next_step_view_app) || ($coverletter_type == 3)) 
    {
			include ("applications/student/add_view_app.inc");
	}

	else 
    {
				include ("applications/student/add_edit_letter.inc");
	}
}

// final confirmation
//elseif (($next_step_insert || $save) && !$already_applied) {
elseif ($next_step_insert || $save) {
		// variables used by coverletter's save.inc
		$letter = urldecode ($app_letter);
		$type_id = 1;
		
		include ('coverletters/save.inc');

		// let's actually insert applications
		$inserting = "Insert into applications (student_number, resume_id, coverletter_id, job_id, transcript, applied_date)
				values ('".addslashes(stripslashes($student_number))."', '".addslashes(stripslashes($resume_id))."', 
				'".addslashes(stripslashes($coverletter_id))."', '".addslashes(stripslashes($job_id))."', 
				'".addslashes(stripslashes($include_transcript))."', '".date('Y-m-d, H:i:s', time())."')";

		if ($GLOBALS['dbh']->Execute ($inserting)) {
			$success = true;
            $sql2 = (" SELECT LAST_INSERT_ID() AS application_id ");
            $result2 = $GLOBALS['dbh']->Execute($sql2);
            $row2 = $result2->FetchRow();
            $application_id = $row2["application_id"];
		}

	//check, just to make sure
	if ($success) 
    {
		notify("Your application to the job:<b> ".$job_row["job_code"].", ". $job_row["position_title"]."</b>
			has been successfully processed.");
	
		echo ("<br />");
        echo("<table width='70%' cellpadding='1' cellspacing='0' border='0' align='center'>");
        echo("<tr>");
            echo("<td>"); 
		        echo ("<form method='post' action='$PHP_SELF'>");
			        echo ("<input type='submit' value='Return to Job Descriptions' />");
		        echo ("</form>");
            echo("</td>"); 
            

            echo("<td>");
                echo ("<form method='post' action='$PHP_SELF'>");
                    echo ("<input type='hidden' name='select' value='applications' />");
                    echo ("<input type='hidden' name='option' value='edit_delete' />");
                    echo ("<input type='hidden' name='application_id' value='".$application_id."' />");
                    echo ("<input type='hidden' name='edit' value=' Edit ' />");
                    echo ("<input type='submit'  value=' Edit Application' />");
                    echo ("</form>");
            echo("</td>"); 

            echo("<td>");
                echo ("<form method='post' action='$PHP_SELF'>");
                    echo ("<input type='hidden' name='application_id' value='".$application_id."' />");
                    echo ("<input type='hidden' name='PDF' value='1' />");
                    echo ("<input type='hidden' name='select' value='studentApplication' />");
                    echo ("<input type='submit' value='View PDF Application' />");
                echo ("</form>");
            echo("</td>");
        echo("</tr>");

        echo("</table>");
    }
    // we should never come here.
    else {
        error ("Error inserting application.");
	}
}


else {

	//if no $next_step means we're coming from job description
	//if (!$next_step && !$already_applied)  {

	// we have to check a bunch of things first

	$student_applicable = false;

	$sql = "SELECT DISTINCT department_id
			FROM student_department
			WHERE student_number='" . $student_number . "'";

	$result = $GLOBALS['dbh']->Execute($sql);

	while ($row = $result->FetchRow())
	{
		/*
		 For each department the student belongs too, check and see if they can apply to this job.
		*/

		$student_applicable = ($student_applicable || isApplicableDept($job_id, $row["department_id"]));
	}

	/*
	 Check and make sure the student has a resume entered into the system.
	*/

	$sql = "SELECT resume_id 
			FROM resume
	        WHERE student_number = '" . addslashes($student_number) . "'";
	$result = $GLOBALS['dbh']->Execute($sql);

	if (!$result->RecordCount())
	{
		$no_resume = true;
	}

	$canStudentApply = canStudentApply($job_id, $student_number);

	if (!$student_applicable)
	{
		error("The job you have attempted to apply to is not applicable to your department.  Please contact your
			Co-op coordinator to receive permission to apply to this job.");
	}
	elseif (!$canStudentApply)
	{
//		error("The job you are trying to apply to is no longer posted, and so you may no longer apply to it.");
	}
	elseif ($no_resume)
	{
		error("You do not yet have a resume entered into the system.  You will need to create a resume before you can apply to jobs.");
	}
    elseif ($userlevel == ADMINSTUDENT)
    {
        error("<h1 class=\"black\">Error</h1><p>You are not allowed to sign a student up for a job from here.  You must log out of the student section and choose \"Add an application\" in the \"Send\" section under \"Job Applications\" to add an application.</p>");
    }
	else
	{
		// display the options for choosing application items
		include ("applications/student/add_choose_items.inc");
	}
}

?>
