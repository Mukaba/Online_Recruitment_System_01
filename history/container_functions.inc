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
 | Filename: container_functions.inc                                            |
 +------------------------------------------------------------------------------+
 | Description: The following are a collection of functions that are used for   |
 |              history containers.                                             |
 +------------------------------------------------------------------------------+

*/
define(ASCII_A, 97);
define(ASCII_Z, 122);
define(CONTAINER_HOURS,1);
define(CONTAINER_TERM,2);

/* predicate to determine whether a student is using containers */
function useContainers() {
    if (func_num_args() == 2)
    {
        $student_number= func_get_arg(0);
        $department_id = func_get_arg(1);

        // determine if the student is in a discipline using containers
        $sql = "SELECT d.use_history_containers_hours, d.use_history_containers_term
                FROM   discipline d
                INNER JOIN student_department sd
                    ON d.discipline_id = sd.discipline_id
                WHERE sd.student_number = '".$student_number."'
                    AND sd.department_id = '".$department_id."'";
        $results  = $GLOBALS['dbh']->Execute($sql);
        $row = $results->FetchRow();
    }
    elseif (func_num_args() == 1)
    {
        $discipline_id = func_get_arg(0);

        // determine if the student is in a discipline using containers
        $sql = "SELECT d.use_history_containers_hours, d.use_history_containers_term
                FROM   discipline d
                WHERE d.discipline_id = '".$discipline_id."'";
        $results  = $GLOBALS['dbh']->Execute($sql);
        $row = $results->FetchRow();
    }
    else
    {
        return -1;
    }

    if ($row['use_history_containers_hours'])
    {
        return CONTAINER_HOURS;
    }
    elseif ($row['use_history_containers_term'])
    {
        return CONTAINER_TERM;
    }
    else 
    {
        return 0;
    }
}

// function to get all the history workterm numbers for a given student
function getAllWorktermNums($student_number, $department_id) {
    $sql= ("
        SELECT h.history_id, h.work_term_number
        FROM history h                                                          
        WHERE h.student_number = '".$student_number."'
        AND h.department_id = '".$department_id."'
        ORDER BY h.start_date
        ");
    $result = $GLOBALS['dbh']->Execute($sql);
    
    while (list($history_id, $workterm_num) = $result->FetchRow()) {
        $workterm_nums[$history_id] = $workterm_num;
    }

    return $workterm_nums;
}

// function to get all the history workterm codes for a given student 
function getAllWorktermCodes($student_number, $department_id, $mode) 
{
    if (!$mode)
    {
        $mode = CONTAINER_TERM;
    }

    if ($mode == CONTAINER_TERM)
    {
        // dump student's history records to array
        $sql = ("
            SELECT h.history_id, h.work_term_length
            FROM history h
            WHERE h.student_number = '".$student_number."'
            AND h.department_id = '".$department_id."'
            ORDER BY h.start_date
            ");
        $result = $GLOBALS['dbh']->Execute($sql);

        while (list($history_id, $workterm_length) = $result->FetchRow()) {
            $workterm_codes[$history_id] = placeInContainer($student_number, $department_id, $workterm_length, $mode);
        }
    }
    elseif ($mode == CONTAINER_HOURS)
    {
        // dump student's history records to array
        $sql = ("
            SELECT h.history_id, h.start_date, h.end_date, h.work_term_hours_per_week
            FROM history h
            WHERE h.student_number = '".$student_number."'
            AND h.department_id = '".$department_id."'
            ORDER BY h.start_date
            ");
        $result = $GLOBALS['dbh']->Execute($sql);

        while (list($history_id, $start_date, $end_date, $work_term_hours_per_week) = $result->FetchRow()) {
            $num_days = count_days($start_date, $end_date);
            $hours_per_day = $work_term_hours_per_week / 7;
            // calculate how many hours the student has worked total in this history record.
            $workterm_length = round($num_days * $hours_per_day);
            $workterm_codes[$history_id] = placeInContainer($student_number, $department_id, $workterm_length, $mode);
        }
    }
    
    return $workterm_codes;
}

/* function to update and synchronize a student's history containers
   with their workterms.  this should be called after workterms have been
   added or deleted. */
function syncContainersWithHistory($student_number, $department_id) {
    // delete and regenerate the container workterm codes in history_container
    $sql = ("
        DELETE FROM history_container
        WHERE student_number = '".$student_number."'
        AND department_id  = '".$department_id."'
        ");
    $GLOBALS['dbh']->Execute($sql);

    $mode = useContainers($student_number, $department_id);

    $all_workterm_codes = getAllWorktermCodes($student_number, $department_id, $mode);

    // no history records left, break out. 
    if (sizeof($all_workterm_codes) == 0) {
        return;
    }
    array_walk($all_workterm_codes, 'updateHistoryContainer', array('student_number' => $student_number,
                                                                    'department_id'  => $department_id));

    // synchronize the history workterm numbers
    $workterm_heads = array_filter($all_workterm_codes, 'headOfContainer');   // workterms at the head of their containers
    $workterm_heads = array_map(create_function('$workterm_codes',            // extract head wt codes
                                                'if (sizeof($workterm_codes) == 1)
                                                    return $workterm_codes[0];
                                                 return $workterm_codes[1];'),
                                $workterm_heads);
    $workterm_heads = array_map(create_function('$workterm_codes',            // drop alphabetic suffixes from WT codes
                                                '
                                                if (is_int($workterm_codes)) {
                                                    return $workterm_codes; 
                                                }
                                                else {
                                                    return preg_replace("/^(\d*)a$/", "$1", $workterm_codes[0]);
                                                }'),
                                $workterm_heads);

    $all_workterm_nums = getAllWorktermNums($student_number, $department_id);
    array_walk($all_workterm_nums, 'updateHistoryWorktermNums', $workterm_heads);
}

/* predicate that returns whether or not a history record is at the head of
   its container given its workterm_codes */
function headOfContainer($workterm_codes) {
    // check for workterm_codes like 1a, 2a, na, or purely numeric codes
    if (sizeof($workterm_codes) > 0) {
        foreach ($workterm_codes as $code) {
            if (preg_match("/^\d*$|^\d*a$/", $code)) {  // eg 1, 2, 1a, 2a, 3a, etc
                return true;
            }
        }
    }
    return false;
}

/* update history wt number given an array of workterms that are at the head of their containers */
function updateHistoryWorktermNums($workterm_num, $history_id, $workterm_heads) {

    if (isset($workterm_heads[$history_id])) {
        $wt_num = $workterm_heads[$history_id];
    } else {
        $wt_num = 'null';
    }

    $sql = ("
        UPDATE history
        SET work_term_number = $wt_num
        WHERE history_id = '".$history_id."'
        ");
    $GLOBALS['dbh']->Execute($sql);
}

/* update work term codes for a history record with workterm_codes */
function updateHistoryContainer($workterm_codes, $history_id, $student_hash) {
    // drop the old workterm_codes
    $GLOBALS['dbh']->Execute("DELETE FROM history_container
              WHERE history_id = '".$history_id."'");

    // reinsert the new ones
    if (sizeof($workterm_codes) > 0) {
        foreach ($workterm_codes as $code) {
            $GLOBALS['dbh']->Execute("INSERT INTO history_container
                      (history_id, workterm_code, student_number, department_id)
                      VALUES ('$history_id', '$code', '".$student_hash['student_number']."', '".$student_hash['department_id']."')");
        }
    }
}

/* helper function for insertHistoryTerm that returns an array of 
   work term codes corresponding to a given workterm
   note the use of static variables to maintain state. 
 */
function placeInContainer($student_num, $department_id, $workterm_length, $mode) {

    static $workterm_number     = 1;
    static $suffix_offset       = 0;  // offset of alphabetic suffix
    static $months_in_container = 0;
    static $hours_in_container  = 0;
    static $student_number = 0;

    if ($student_number != $student_num)
    {
        $workterm_number     = 1;
        $suffix_offset       = 0;  // offset of alphabetic suffix
        $months_in_container = 0;
        $hours_in_container = 0;
    }

    $student_number = $student_num;
    
    $workterm_codes = array();

    if ($mode == CONTAINER_TERM)
    {
        $sql = ("
            SELECT d.history_containers_length_term
            FROM student_department sd
            INNER JOIN discipline d
            ON sd.discipline_id = d.discipline_id
            WHERE sd.student_number = '".$student_number."' AND sd.department_id = '".$department_id."'
            ");
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();

        $CONTAINER_LENGTH = $row['history_containers_length_term'];
        assert($CONTAINER_LENGTH);
        /// cases to consider:
        ///  1. workterm-length == $CONTAINER_LENGTH and it fits a container beginning to end exactly
        ///     and it gets a normal numeric workterm number
        ///  2. workterm-length < $CONTAINER_LENGTH and it gets a numeric+alphabetic workterm code
        ///       i. the workterm-length spills over to the next container, which means 
        ///          we need to return two workterm_codes that will be associated with the
        ///          particular workterm

        if ($workterm_length == $CONTAINER_LENGTH && $suffix_offset == 0) {
            $workterm_codes[] = $workterm_number++;
        } else {
            $workterm_codes[] = $workterm_number . chr(ASCII_A + $suffix_offset);

            $months_in_container += $workterm_length;

            // workterm spans two containers
            if ($months_in_container > $CONTAINER_LENGTH) {
                // move to the next container
                //$months_in_container = 0;
                $months_in_container = $months_in_container - $CONTAINER_LENGTH;
                $suffix_offset       = 0;
                $workterm_codes[]    = ++$workterm_number . chr(ASCII_A);
            }
            // work term fits exactly in a container
            if ($months_in_container == $CONTAINER_LENGTH)
            {
                $suffix_offset = 0;
                $months_in_container = 0;
                $workterm_number++;
            }
            else
            {
                $suffix_offset = ($suffix_offset + 1) % $CONTAINER_LENGTH;  // modulo arithmetic to calculate offset
            }
        }
    }
    elseif ($mode == CONTAINER_HOURS)
    {
        $sql = ("
            SELECT d.history_containers_length_hours, d.history_containers_spill_over_hours
            FROM student_department sd
            INNER JOIN discipline d
            ON sd.discipline_id = d.discipline_id
            WHERE sd.student_number = '".$student_number."' AND sd.department_id = '".$department_id."'
            ");
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();

        $CONTAINER_LENGTH = $row['history_containers_length_hours'];
        $CONTAINER_BUFFER = $row['history_containers_spill_over_hours'];
        assert($CONTAINER_LENGTH);

        /// cases to consider:
        ///  1. workterm-length == $CONTAINER_LENGTH and it fits a container beginning to end exactly
        ///     and it gets a normal numeric workterm number
        ///  2. workterm-length < $CONTAINER_LENGTH and it gets a numeric+alphabetic workterm code
        ///       i. the workterm-length spills over to the next container, which means 
        ///          we need to return two workterm_codes that will be associated with the
        ///          particular workterm

        // when we get work terms by hours, we are usually never dead on the container_length, so we give it a 10 hour buffer. 
        while ((($workterm_length + $CONTAINER_BUFFER) >= $CONTAINER_LENGTH) && $suffix_offset == 0) {
            $workterm_codes[] = $workterm_number++;
            $workterm_length -= $CONTAINER_LENGTH;

            // the remaining workterm length is within the 10 hour buffer, so no sense in creating an extra history container. 
            if ($workterm_length <= $CONTAINER_BUFFER) {
                $workterm_length = 0;
            }
        }
    
        if ($workterm_length > 0) {
            $workterm_codes[] = $workterm_number . chr(ASCII_A + $suffix_offset);

            $hours_in_container += $workterm_length;

            // workterm spans two containers
            if ($hours_in_container >= $CONTAINER_LENGTH) {
                // move to the next container
                $hours_in_container = 0;
                $suffix_offset       = 0;
                $workterm_codes[]    = ++$workterm_number . chr(ASCII_A);
            }

            $suffix_offset = ($suffix_offset + 1) % 4;
        }
    }

    return $workterm_codes;
}

// function to get the next work term code for the student. The returned code is what work term code the history record would get if one were entered right now. 
function getNextWorktermCode($student_number, $department_id, $mode) 
{
    assert($mode);

    if ($mode == CONTAINER_TERM)
    {
        // dump student's history records to array
        $sql = ("
            SELECT h.history_id, h.work_term_length
            FROM history h
            WHERE h.student_number = '".$student_number."'
            AND h.department_id = '".$department_id."'
            ORDER BY h.start_date
            ");
        $result = $GLOBALS['dbh']->Execute($sql);

        while (list($history_id, $workterm_length) = $result->FetchRow()) {
            $workterm_codes[] = placeInContainer($student_number, $department_id, $workterm_length, $mode);
        }
        // see what work term code we get if we were to add another history record 
        $workterm_codes[] = placeInContainer($student_number, $department_id, 1, $mode);
    }
    elseif ($mode == CONTAINER_HOURS)
    {
        // dump student's history records to array
        $sql = ("
            SELECT h.history_id, h.start_date, h.end_date, h.work_term_hours_per_week
            FROM history h
            WHERE h.student_number = '".$student_number."'
            AND h.department_id = '".$department_id."'
            ORDER BY h.start_date
            ");
        $result = $GLOBALS['dbh']->Execute($sql);

        while (list($history_id, $start_date, $end_date, $work_term_hours_per_week) = $result->FetchRow()) {
            $num_days = count_days($start_date, $end_date);
            $hours_per_day = $work_term_hours_per_week / 7;

            // calculate how many hours the student has worked total in this history record.
            $workterm_length = round($num_days * $hours_per_day);
            $workterm_codes[] = placeInContainer($student_number, $department_id, $workterm_length, $mode);
        }
        // see what work term code we get if we were to add another history record 
        $workterm_codes[] = placeInContainer($student_number, $department_id, 1, $mode);
    }
    
    $code = $workterm_codes[(sizeof($workterm_codes)-1)][0];

    // If a code looks like 1a, 2a, we strip off the letter. 
    if (preg_match("/^\da$/",$code))
    {
        $code = (int) $code;
    }

    return $code;
}

?>
