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
 | Filename: functions.inc                                                      |
 +------------------------------------------------------------------------------+
 | Description: This file contains functions related to the student module.     |
 +------------------------------------------------------------------------------+

*/

function student_flags($recordid,$departmentid)
{
	$count=0;
	//get flags that the particular student has set
	$sql="SELECT description 
	      FROM student_flags as sf,student_flags_join as sfj, department_student_flags as dsf 
	      WHERE dsf.student_flags_id=sf.student_flags_id AND dsf.department_id='".$departmentid."' AND sf.student_flags_id=sfj.student_flags_id 
	            AND sfj.record_id='".$recordid."' 
	      ORDER BY sf.student_flags_id";    	
	$markedflags=$GLOBALS['dbh']->Execute($sql);
	$sql2 = "SELECT description 
             FROM student_flags AS sf,department_student_flags AS dsf 
             WHERE dsf.student_flags_id=sf.student_flags_id and dsf.department_id='".$departmentid."' 
             ORDER BY sf.student_flags_id";

	//get all student flags in our department
	$allflags=$GLOBALS['dbh']->Execute($sql2);
	$row2=$markedflags->FetchRow();
	while ($row=$allflags->FetchRow())
    {
		if ($row2["description"]!=$row["description"])
        {
			$anarray[$count+1] = "&nbsp";
		}
        
        else
        {
			$row2=$markedflags->FetchRow();
			$anarray[$count+1]="<img src='misc/images/smchk_bl.gif' alt='' />";
		}
		$anarray[$count]=$row["description"];
		$count=$count+2;
	}
    return $anarray;
}

function student_search_flags($student_num,$departmentid)
{
        $count=0;
        $sql = "SELECT description, sf.student_flags_id 
                FROM student_flags AS sf,department_student_flags AS dsf 
                WHERE dsf.student_flags_id=sf.student_flags_id AND dsf.department_id='".$departmentid."' AND sf.flag_type_id != '".ELIGIBLE_FLAG."' 
                ORDER BY description";
        $results=$GLOBALS['dbh']->Execute($sql);
        while ($row=$results->FetchRow())
        {
                $anarray[$count+1] = "&nbsp";
                $anarray[$count]=$row["description"];
                $count=$count+2;
        }
        return $anarray;
}

/* 
 The function below returns an array related to all of the flags (of the specified type) for the student that matches the
 specified student.  The array that is returned is an associative one.  The keys are as follows:
 "flag_name" is the name of the flag (that will get parsed and displayed by editstudent.inc
 "flag_value" is the boolean value of the flag (true if it's set, false otherwise).
 "flag_id" is the primary key that represents the specific flag.
*/

function get_student_flags($record_id, $student_department_id, $flag_type_id)
{
	$sql = ("
        SELECT esf.order_by, CONCAT(t.term_name, \" \", " . date('Y') . " + esf.year_space) AS name, sf.student_flags_id,
            sf.description, ep.term_id, ep.year, sfj.record_id AS flag_set, sf.flag_type_id 
        FROM student_flags AS sf
        INNER JOIN department_student_flags AS dsf ON dsf.student_flags_id=sf.student_flags_id
        INNER JOIN special_flags AS spf ON spf.flag_type_id=sf.flag_type_id
        LEFT JOIN student_flags_join AS sfj ON sf.student_flags_id=sfj.student_flags_id AND sfj.record_id='" . $record_id . "' 
        LEFT JOIN edit_student_flags AS esf ON esf.student_flags_id=sf.student_flags_id 
        LEFT JOIN eligible_placed AS ep ON ep.student_flags_id=sf.student_flags_id AND ep.term_id=esf.term_id AND ep.year=esf.year_space + " . date('Y') . " AND ep.record_id='".$record_id."' 
        LEFT JOIN term AS t ON t.term_id=esf.term_id 
        WHERE dsf.department_id='" . $student_department_id . "' 
		AND sf.flag_type_id='" . $flag_type_id . "' 
        ORDER BY esf.order_by, t.term_id, sf.description
        ");
	$results = $GLOBALS['dbh']->Execute($sql);
	$count = 0;
	while($row = $results->FetchRow())
    {
		if (!($flag_type_id == TERM_FLAGS && $row["name"] == NULL))
        {
			if ($flag_type_id == TERM_FLAGS)
            {
				if ($row["term_id"] != NULL and $row["year"] != NULL)
                {
                    $flag_array[$count]["flag_value"] = true;
				}
                else
                {
                    $flag_array[$count]["flag_value"] = false;
				}
			}
            else
            {
				if ($row["flag_set"] != NULL)
                {
                    $flag_array[$count]["flag_value"] = true;
				}
                else
                {
                    $flag_array[$count]["flag_value"] = false;
				}
			}

			if ($flag_type_id == TERM_FLAGS)
            {
                $flag_array[$count]["flag_name"] = $row["description"] . " (" . $row["name"] . ")";
			}
            else
            {
                $flag_array[$count]["flag_name"] = $row["description"];
            }

            $flag_array[$count]["flag_id"] = $row["student_flags_id"];
			$count++;
		}
	}
    return $flag_array;
}

?>
