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
 | Filename: flag.inc                                                           |
 +------------------------------------------------------------------------------+
 | Description: needs to save the flags for this student. Note: flag.inc        |
 | relies on general_display.inc . general_display.inc provides an associates   |
 | array called $ep_array which contains variables required to make a student   |
 | eligible in the eligible_placed table. general_display.inc also provides a   |
 | variable called $max_student_flags_id that's required for eligible status.   |
 +------------------------------------------------------------------------------+

*/
if (!$continue)
{
	// They haven't done anything yet, so display the general form for them.
	
	include('student_info/set/general_display.inc');
}

elseif ($continue == "Set Flag and Save for All Students on All Pages")
{
    $associative_array = "";
    for ($i=$max_student_flags_id+1;$i<sizeof($ep_array)+$max_student_flags_id+1;$i++)
    {
        $associative_array .= ("<input type='hidden' name='ep_array[".$i."][student_flags_id]' value='".ELIGIBLE_FLAG."' />");
        $associative_array .= ("<input type='hidden' name='ep_array[".$i."][term_id]' value='".$ep_array[$i]['term_id']."' />");
        $associative_array .= ("<input type='hidden' name='ep_array[".$i."][term_name]' value='".$ep_array[$i]['term_name']."' />");
        $associative_array .= ("<input type='hidden' name='ep_array[".$i."][year]' value='".$ep_array[$i]['year']."' />");
    }

    if ($student_flag > $max_student_flags_id)
    {
        $flag_name = "Eligible ".$ep_array[$student_flag]['term_name']." ".$ep_array[$student_flag]['year'];
    }
    else
    {
        $flag_sql = ("
            SELECT description
            FROM student_flags
            WHERE student_flags_id = '".$student_flag."'
            ");
        $flag_name = $GLOBALS['dbh']->GetOne($flag_sql);
    }
    $student_id_list = urldecode($student_id_list);
    $count_array = explode(",",$student_id_list);
    $student_text = (sizeof($count_array) > 1) ? "s" : "";
    instruct("Are you sure you want to set the ".$flag_name." flag for ".sizeof($count_array)." student".$student_text."?
              <form method='post' action='".$PHP_SELF."&amp;select=set_student_flags'>
              ".$associative_array."
              <input type='hidden' name='max_student_flags_id' value='".$max_student_flags_id."' />
              <input type='hidden' name='student_flag' value='".$student_flag."' />
              <input type='hidden' name='student_id_list' value='".urlencode($student_id_list)."' />
              <input type='hidden' name='form_submitted' value='true' />
              <input type='hidden' name='start_row' value='".$start_row."' />
              <input type='hidden' name='first_time' value='0' />
              <input type='submit' name='continue' value='Yes, set all flags' />&nbsp;&nbsp;
              <input type='submit' name='continue' value='No' />
              </form>");
}

elseif ($continue == 'save_specific')
{

    // The user would like to save the changes they have made to this student's flags.


    $contact_flags = trim_array($contact_flags);
    $department_contact_flags = unpackObject($department_contact_flags);


    // Delete all of the old flags (that this department can set) for this student from the database.


    for ($i = 0; $i < sizeof($department_contact_flags); $i++)
    {
        $sql = ("
            DELETE FROM student_flags_join
            WHERE record_id='" . $record_id . "'
            AND student_flags_id='" . $department_contact_flags[$i] . "'
            ");
        $result = $GLOBALS['dbh']->Execute($sql);
    }


    // Insert all of the new flags that have been set for this student into the database.


    for ($i = 0; $i < sizeof($contact_flags); $i++)
    {
        // :TODO: This block of code doesn't actually do anything useful. Refactor when you get time 
        if ($contact_flags[$i] == ELIGIBLE_FLAG)
        {
            $term = date("m");
            $year = date("Y");

            $sql = "SELECT term_id
                    FROM term
                    WHERE start_date <= '".$term."' AND end_date >= '".$term."'";
            $results = $GLOBALS['dbh']->Execute($sql);
            if ($results->RecordCount() == 1)
            {
                $row=$results->FetchRow();

            }

            else
            {
                error("An error has occurred while setting the eligible flag for your students.");
            }

        }

        else
        {
            $sql = ("
                INSERT INTO student_flags_join
                (record_id, student_flags_id)
                VALUES
                (
                '" . $record_id . "',
                '" . $contact_flags[$i] . "'
                )
                ");
        }
        $result = $GLOBALS['dbh']->Execute($sql);
    }

    unset($department_contact_flags);
    unset($contact_flags);


    // Notify that the changes have been successfully made, and reload the page.

    notify("Flags have been saved successfully.");
    include('student_info/set/general_display.inc');
}

elseif ($continue == "Unset Flag and Save for All Students on All Pages")
{
    $associative_array = "";
    for ($i=$max_student_flags_id+1;$i<sizeof($ep_array)+$max_student_flags_id+1;$i++)
    {
        $associative_array .= ("<input type='hidden' name='ep_array[".$i."][student_flags_id]' value='".ELIGIBLE_FLAG."' />");
        $associative_array .= ("<input type='hidden' name='ep_array[".$i."][term_id]' value='".$ep_array[$i]['term_id']."' />");
        $associative_array .= ("<input type='hidden' name='ep_array[".$i."][term_name]' value='".$ep_array[$i]['term_name']."' />");
        $associative_array .= ("<input type='hidden' name='ep_array[".$i."][year]' value='".$ep_array[$i]['year']."' />");
    }

    if ($student_flag > $max_student_flags_id)
    {
        $flag_name = "Eligible ".$ep_array[$student_flag]['term_name']." ".$ep_array[$student_flag]['year'];
    }
    else
    {
        $flag_sql = ("
            SELECT description
            FROM student_flags
            WHERE student_flags_id = '".$student_flag."'
            ");
        $flag_name = $GLOBALS['dbh']->GetOne($flag_sql);
    }

    $student_id_list = urldecode($student_id_list);
    $count_array = explode(",",$student_id_list);
    $student_text = (sizeof($count_array) > 1) ? "s" : "";
    instruct("Are you sure you want to unset the ".$flag_name." flag for ".sizeof($count_array)." student".$student_text."?
              <form method='post' action='".$PHP_SELF."&amp;select=set_student_flags'>
              ".$associative_array."
              <input type='hidden' name='max_student_flags_id' value='".$max_student_flags_id."' />
              <input type='hidden' name='student_flag' value='".$student_flag."' />
              <input type='hidden' name='student_id_list' value='".urlencode($student_id_list)."' />
              <input type='hidden' name='form_submitted' value='true' />
              <input type='hidden' name='start_row' value='".$start_row."' />
              <input type='hidden' name='first_time' value='0' />
              <input type='submit' name='continue' value='Yes, unset all flags' />&nbsp;&nbsp;
              <input type='submit' name='continue' value='No' />
              </form>");

}

elseif ($continue == 'Save Changes' || $continue == 'Yes, set all flags' || $continue == 'Yes, unset all flags')
{
	// They want to save some flags that they have set.  We have been passed back three variables from
	// the form: flag_id, which is the flag they were setting or unsetting, student_ids, which is an
	// array containing all of the student id's that they were setting and unsetting for, and
	// flag_on, which is an array containing the student id's that have the flag set to on.

	if ($student_ids && is_string($student_ids))
	{
		$student_ids = unpackObject($student_ids);
        $student_ids = explode(",",$student_ids);
	}
    
    elseif ($student_id_list && is_string($student_id_list) && $continue == 'Yes, set all flags')
    {
        $student_id_list = urldecode($student_id_list);
        $student_ids = explode(",",$student_id_list);
        $flag_on = $student_ids;
    }

    elseif ($student_id_list && is_string($student_id_list) && $continue == 'Yes, unset all flags')
    {
        $student_id_list = urldecode($student_id_list);
        $student_ids = explode(",",$student_id_list);
        $flag_on = NULL;
    }

	// Trim out the blank elements of flag_on.
	

    if ($flag_on && is_array($flag_on))
    {
        $flag_on = trim_array($flag_on);
    }
	
	// Delete all the instances of this flag and student id from the database.

    // we're dealing with eligible flags
    if ($student_flag > $max_student_flags_id)
    {
        include('student_info/edit/EditStudent.class');
        $editStudent = new EditStudent('');

        // make a copy of flag on. We need a copy because all eligible flags are deleted from a student, and they can be re-inserted into the database. 
        // So, what we do is, we delete an eligible flag for each student, and we see if that student's record id shows up again in the flag_on array. 
        // If it does, that means the eligible flag is going to be restored, so no need to disable the student's job applications. If this is the case,
        // we set that student's record id in c_flag_on to null. Any remaining record ids in the c_flag_on array means those students didn't have eligible
        // flags before, and we're adding them in now, so we need to enable their job applications. 
        $c_flag_on = $flag_on;
    }

	for ($i = 0; $i < sizeof($student_ids); $i++)
	{
        if ($student_flag > $max_student_flags_id)
        {
            $sql = ("
                DELETE
                FROM eligible_placed
                WHERE record_id = '".$student_ids[$i]."' AND student_flags_id = '".$ep_array[$student_flag]['student_flags_id']."'
                AND term_id = '".$ep_array[$student_flag]['term_id']."' AND year = '".$ep_array[$student_flag]['year']."'
                ");
            $result = $GLOBALS['dbh']->Execute($sql);
            if ($GLOBALS['dbh']->Affected_Rows($result)) 
            {
                // We deleted an eligible flag, and that flag is not going to be restored. Thus, we made a student ineligble, disable that student's
                // job applications. 
                if (is_array($c_flag_on))
                {
                    $key = array_search($student_ids[$i],$c_flag_on);
                }

                // === does a comparison based on type AND value. See php.net for more info. 
                if ($key === false || sizeof($c_flag_on) == 0)
                {
                    $sql = ("
                        SELECT student_number
                        FROM student_department
                        WHERE record_id = '".$student_ids[$i]."'
                        ");
                    $result = $GLOBALS['dbh']->Execute($sql);
                    $row = $result->FetchRow();
                    $temp_student_number = $row['student_number'];
                    $editStudent->disableJobApplications($temp_student_number, $ep_array[$student_flag]['term_id'], $ep_array[$student_flag]['year']);
                }
                else
                {
                    $c_flag_on[$key] = null;
                }
            }
        }

        else 
        {
            $sql = ("
                DELETE FROM student_flags_join
                WHERE record_id='" . $student_ids[$i] . "'
                AND student_flags_id='" . $student_flag . "'
                ");
            $GLOBALS['dbh']->Execute($sql);
        }
	}

	// Insert back in all of the new instances of this flag and student_id.
	
	if (trim($flag_on) && sizeof($flag_on) > 0 && is_array($flag_on))
	{
		for ($i = 0; $i < sizeof($flag_on); $i++)
		{
            if ($student_flag > $max_student_flags_id)
            {
                $sql = ("
                    INSERT
                    INTO eligible_placed
                    (record_id, student_flags_id, term_id, year)
                    VALUES
                    ('".$flag_on[$i]."','".$ep_array[$student_flag]['student_flags_id']."','".$ep_array[$student_flag]['term_id']."'
                    ,'".$ep_array[$student_flag]['year']."')
                    ");
                $GLOBALS['dbh']->Execute($sql);

                if (!wasEverEligible($flag_on[$i], $ep_array[$student_flag]['term_id'], $ep_array[$student_flag]['year'])) {
                    setEverEligible($flag_on[$i], $ep_array[$student_flag]['term_id'], $ep_array[$student_flag]['year']);
                }

                if ($c_flag_on[$i])
                {
                    $sql = ("
                        SELECT student_number
                        FROM student_department
                        WHERE record_id = '".$flag_on[$i]."'
                        ");
                    $result = $GLOBALS['dbh']->Execute($sql);
                    $row = $result->FetchRow();
                    $temp_student_number = $row['student_number'];
                    $editStudent->enableJobApplications($temp_student_number, $ep_array[$student_flag]['term_id'], $ep_array[$student_flag]['year']);
                }
            }

            else 
            {
                //This is to resolve the duplication of flags. Where this bug originated is still a mystery.
                $sql2 = ("
                    SELECT * 
                    FROM student_flags_join
                    WHERE record_id = '" . $flag_on[$i] . "'". "  
                    AND student_flags_id = '" . $student_flag . "'"
                    );
                
                $result = $GLOBALS['dbh']->Execute($sql2);
                if ($result->RecordCount()==0)
                { 
                    $sql = ("
                        INSERT INTO student_flags_join
                        (record_id, student_flags_id)
                        VALUES 
                        (
                         '" . $flag_on[$i] . "',
                         '" . $student_flag . "'
                        )
                        ");
                    $GLOBALS['dbh']->Execute($sql);
                }
            }
        }
	}

	
	// Reload the previous page.

	notify("Flags have been saved successfully.");
	include('student_info/set/general_display.inc');
}

elseif ($continue == 'No')
{
    $student_ids = unpackObject($student_ids);
    $form_submitted = 'true';
    $first_time = 0;
    include('student_info/set/general_display.inc');
}

else
{
    notify("An error has occurred while setting flags.");
	include('student_info/set/general_display.inc');
}

?>
