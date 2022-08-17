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
 | Description: if the job form hasn't been filled out, go to the job form.     |
 +------------------------------------------------------------------------------+

*/

if(($continue != "Shortlist All Students") && ($continue != "Shortlist Selected Students"))
{
?>

    <!-- Close the header -->
    </td><td align="left" valign="top"><img src="<?php echo $GLOBALS['colorscheme']['topbar_right']; ?>" alt='' /></td></tr></table>

<?php
}

// Interview class declaration to aid passing of input between forms.
include('interview/add/AddInterview.class');

assert( define("INTERVIEW", 1) );
assert( define("INT_BREAK", 2) );
assert( define("LUNCH",3) );
assert( define("NO_TIME",4) );

echo("<table border='0' cellspacing='0' cellpadding='2' width='99%'><tr><td>\n");
echo("<h3 align='left'>Interviews - Add</h3>");

if ($dup_students_solved && !$interview)
{
    include('interview/add/add_job_form.inc');
}
elseif ($submit == 'View')
{
    include('interview/add/existing_job_choices.inc');
}
elseif ((!$job_select && !$submit) || $change_limit) 
{
    include('interview/add/existing_job_choices.inc');
} 
elseif ($existing_job_interview)
{
    // Do some error checking here.
    $jobs_for_interview = trim_array($jobs_for_interview);

    if (!$jobs_for_interview)
    {
        error("You must enter a job code, or choose from one of the jobs below to proceed.");
        include('interview/add/existing_job_choices.inc');
    }
    else
    {
        foreach($jobs_for_interview AS $jfi)
        {
            $employer_id = getJobEmployer($jfi);

            if ($old_employer_id && ($employer_id != $old_employer_id))
            {
                $employers_dont_match = true;
            }
            else
            {
                $old_employer_id = $employer_id;
            }
        }

        if ($employers_dont_match)
        {
            error("You can only create an interview for multiple jobs if those jobs are all for the same employer.");
            include('interview/add/existing_job_choices.inc');
        }
        else
        {
            // We now have all of the job ids associated with this interview.
            // Save the job ID and put create an interview object with this data.

            $interview = new AddInterview('');

            if (sizeof($jobs_for_interview) == 1)
            {
                // get the shortlisted students (if any)
                $sql = "SELECT student_number
                    FROM students_shortlisted
                    WHERE job_id = '". $jobs_for_interview[0] ."'";
                $result = $GLOBALS['dbh']->Execute ($sql);
                while ($row = $result->FetchRow())
                    $students .= getStudentName ($row["student_number"]) ." \n";

                $interview->job_id = $jobs_for_interview[0];
                $job_codes = getIntJobCode($interview->job_id);
                $interview->job_code = $job_codes[0];

                $employer_id = getJobEmployer($jobs_for_interview[0]);
                $company_name = getCompanyName($employer_id);
                $interview->employer_id = $employer_id;
                $interview->company_name = $company_name;
                $interview->existing_single_job = true;
                $interview->students = $students;

                include('interview/add/add_job_form.inc');
            }
            else
            {

                // Keep track of the multiple job ids that relate to this one interview.
                $interview->multi_job_interview = true;
                foreach($jobs_for_interview AS $ji)
                {
                    // get the shortlisted students (if any)
                    $sql = "SELECT student_number
                        FROM students_shortlisted
                        WHERE job_id = '". $ji ."'";
                    $result = $GLOBALS['dbh']->Execute ($sql);
                    while ($row = $result->FetchRow())
                        $students .= getStudentName ($row["student_number"]) ." \n";

                    $interview->multi_job_ids .= ($ji . ",");

                    $sql = ("
                            SELECT DISTINCT job_code
                            FROM job_info
                            WHERE job_id='" . $ji . "'
                            ");
                    $result = $GLOBALS['dbh']->Execute($sql);
                    $row = $result->FetchRow();
                    $interview->multi_job_codes[] = $row["job_code"];
                }
                $interview->multi_job_ids = substr($interview->multi_job_ids, 0, -1);

                // Get the job codes for this interview.
                foreach ($interview->multi_job_codes AS $jc)
                {
                    $job_code .= ($jc . ", ");
                }
                $job_code = substr($job_code, 0, -2);
                $interview->job_code = $job_code;

                $employer_id = getJobEmployer($jobs_for_interview[0]);
                $company_name = getCompanyName($employer_id);
                $interview->employer_id = $employer_id;
                $interview->company_name = $company_name;
                $interview->students = $students;

                include('interview/add/add_job_form.inc');
            }
        }
    }
}
elseif ($provided_job_code)
{
    if (!trim($job_code))
    {
        error("You must enter a job code, or choose from one of the jobs below to proceed.");
        include('interview/add/existing_job_choices.inc');
    }
    else
    {
        // We've been given a job code, check and make sure it doesn't already exist, and if not save this in job_info.
        $current_term = getCurrentTermID();
        $current_year = getCurrentYear();

        // Get the date of the start of this term.  No duplicate job codes are allowed from that date onwards.
        $sql = ("
            SELECT DISTINCT start_date
            FROM term
            WHERE term_id='" . $current_term . "'
            ");
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();
        $term_start_month = $row["start_date"];
        $term_start_date = date("Y-m-d", mktime(0, 0, 0, $term_start_month, 1, $current_year));

        $sql = ("
            SELECT DISTINCT it.job_id
            FROM interview_time AS it, job_info AS ji
            WHERE ji.job_id=it.job_id
            AND ji.job_code='" . addslashes($job_code) . "'
            AND it.int_date>='" . $term_start_date . "'
            ");
        $result = $GLOBALS['dbh']->Execute($sql);
        //:TODO Finish checking to see that this works.

        if ($result->RecordCount())
        {
            $error = 1;
            $row = $result->FetchRow();
            $job_id = $row["job_id"];

            $msg = ("Interview <b class='instruct'>" . $job_code . "</b> has already been scheduled for ");

            $result = $GLOBALS['dbh']->Execute("
                SELECT DISTINCT int_date 
                FROM interview_time 
                WHERE job_id='" . $job_id . "'
                ");
            while(($row = $result->FetchRow()))
            {
                $msg .= ("<b class='instruct'>" . $row[int_date] . "</b>&nbsp;");
            }

            $msg .= ("<br /><br />If this interview is part of a multipart interview, it is suggested that you create a new interview using the same job code, ");
            $msg .= ("but append <b>-#</b> to the end of it, where <b>#</b> is the number of the part of the interview.  Eg.  If this is part 2 of an ");
            $msg .= ("interview for job code 2002F-00001, then use 2002F-00001-2 as the job code for this interview.");
            notify($msg);

            include('interview/add/existing_job_choices.inc');
        }
        else
        {
            // This is ridiculous, I know.
            if ($current_term == '1')
            {
                $date_chk_query = ("
                        AND year>'" . $current_year . "'
                        ");
            }
            elseif ($current_term == '2')
            {
                $date_chk_query = ("
                        AND (year>'" . $current_year . "'
                            OR (year='" . $current_year . "' AND term_id IN ('1', '3')))
                        ");
            }
            elseif ($current_term == '3')
            {
                $date_chk_query = ("
                        AND (year>'" . $current_year . "'
                            OR (year='" . $current_year . "' AND term_id='1'))
                        ");
            }

            $sql = ("
                    SELECT DISTINCT ji.position_title, ji.job_id, ji.job_code, ji.status, ji.term_id, ji.year, ec.company_name, ed.department_name
                    FROM job_info AS ji, employer_company AS ec
                    LEFT JOIN employer_department AS ed ON ji.employer_department_id=ed.department_id
                    WHERE ji.job_code='" . $job_code . "'
                    AND ec.employer_id=ji.employer_id
                    AND ji.status!='" . INT_ONLY . "'
                    AND ji.status!='" . REJECTED . "'
                    AND ji.status!='" . CANCELLED . "'
                    ");
            $sql .= $date_chk_query;

            $result = $GLOBALS['dbh']->Execute($sql);

            if ($result->RecordCount() && !$add_confirmed) 
            {
                $row = $result->FetchRow();
                $job_id = $row["job_id"];

                $msg = ("There is already a job in the database with the job code <b>" . $row["job_code"] . "</b>.");
                $msg .= ("  Below are the details:<br /><br />");
                $msg .= ("Position title: <b>" . $row["position_title"] . "</b><br />");
                $msg .= ("Company name: <b>" . $row["company_name"] . "</b><br />");
                $msg .= (($row["department_name"]) ? "Division name: <b>" . $row["department_name"] . "</b><br />" : "");
                $msg .= ("Scheduled for: <b>" . getTermName($row["term_id"]) . "</b>, <b>" . $row["year"] . "</b><br />");
                $msg .= ("Status: <b>" . getStatusName($row["status"]) . "</b>");
                $msg .= ("<br /><br />Are you sure you would like to add a new interview, independant from this job?");
                promptAdd( $msg, $job_code );
            }
            else
            {
                $interview = new AddInterview('');
                $interview->job_code = addslashes(trim($job_code));
                $interview->interview_only = true;

                include('interview/add/add_job_form.inc');
            }
        }
    }
}
elseif (!$submit)
{
    // Create an interview object, and get a job id for it.
    $interview = new AddInterview('');
    $interview->job_code = addslashes(trim($job_code));
    $interview->interview_only = true;

    include('interview/add/add_job_form.inc');
}
else
{
    include('interview/add/add_job_form.inc');
}

echo ("</td>");
echo("</tr>");
echo("</table>");

function promptAdd( $msg, $job_code )
{
    global $PHP_SELF;

    $msg .= ("<table cellspacing='2' cellpadding='2'>");
    $msg .= ("<tr>");
    $msg .= ("<td>");
    $msg .= ("<form method='post' action='" . $PHP_SELF . "'>");
    $msg .= ("<input type='hidden' name='job_code' value='" . $job_code . "' />");
    $msg .= ("<input type='hidden' name='add_confirmed' value='true' />");
    $msg .= ("<input type='hidden' name='select' value='add' />");
    $msg .= ("<input type='hidden' name='provided_job_code' value='true' />");
    $msg .= ("<input type='hidden' name='job_select' value='true' />");
    $msg .= ("<input style='width: 5em;' type='submit' value='Yes' />");
    $msg .= ("</form>");
    $msg .= ("</td>");

    $msg .= ("<td>");
    $msg .= ("<form method='post' action='" . $PHP_SELF . "'>");
    $msg .= ("<input type='hidden' name='select' value='add' />");
    $msg .= ("<input style='width: 5em;' type='submit' value='No' />");
    $msg .= ("</form>");
    $msg .= ("</td>");
    $msg .= ("</tr>");
    $msg .= ("</table>");

    error($msg);
}

?>
