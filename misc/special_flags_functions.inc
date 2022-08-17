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
 | Filename: special_flags_functions.inc                                        |
 +------------------------------------------------------------------------------+
 | Description: When a student is placed, we delete the student's eligible      |
 | flag and replace it with a placed flag. To-do: This file needs to be merged  |
 | with misc/functions.inc, there's only one place where file is used, and      |
 | that's save_placements.inc Last Modified: -CHaN                              |
 +------------------------------------------------------------------------------+

*/

//:TODO: This file is obsolete, we should delete this. 

//Checks for special case flags and performs the appropiate action
function check_special_flags($flag_id,$record_id,$term_id,$year,$action)
{
	if ($action=="Insert")
    {
		switch($flag_id)
		{
			case ELIGIBLE_FLAG:
				if (($term_id==NULL || $term_id==0) && ($year==NULL || $year==0))
                {
					$sql = "SELECT term_id 
                            FROM term 
                            WHERE start_date<='".date(m)."' AND end_date >= '".date(m)."'";
					$results=$GLOBALS['dbh']->Execute($sql);
					$row=$results->FetchRow();
					$term_id=$row['term_id'];
					$year=date(Y);
				}
				$sql = "INSERT INTO eligible_placed 
                        (record_id,term_id,year,student_flags_id) 
                        VALUES 
                        ('".$record_id."','".$term_id."','".$year."','".$flag_id."')";
				$GLOBALS['dbh']->Execute($sql);
				reset_special_flags(PLACED_FLAG,$record_id,$term_id,$year);
				break;
            case PLACED_FLAG:
				if (($term_id==NULL || $term_id==0) && ($year==NULL || $year==0))
                {
					$sql = "SELECT term_id 
                            FROM term 
                            WHERE start_date<='".date(m)."' AND end_date >= '".date(m)."'";
                    $results=$GLOBALS['dbh']->Execute($sql);
                    $row=$results->FetchRow();
                    $term_id=$row['term_id'];
					$year=date(Y);
				}
				$sql = "INSERT INTO eligible_placed 
                        (record_id,term_id,year,student_flags_id) 
                        VALUES 
                        ('".$record_id."','".$term_id."','".$year."','".$flag_id."')";
				$GLOBALS['dbh']->Execute($sql);
				reset_special_flags(ELIGIBLE_FLAG,$record_id,$term_id,$year);
                break;
		}
	}
}
//Unsets the flags that are used for historical purposes.
function reset_special_flags($flag_id,$record_id,$term_id,$year)
{
    if (($term_id==NULL || $term_id==0) && ($year==NULL || $year==0))
    {
		$sql = "SELECT term_id 
                FROM term 
                WHERE start_date<='".date(m)."' AND end_date >= '".date(m)."'";
		$results=$GLOBALS['dbh']->Execute($sql);
		$row=$results->FetchRow();
		$term_id=$row['term_id'];
		$year=date(Y);
	}
	$sql = "DELETE FROM student_flags_join WHERE record_id='".$record_id."' AND student_flags_id='".$flag_id."'"; 
	$GLOBALS['dbh']->Execute($sql);
	$sql = "DELETE FROM eligible_placed WHERE record_id='".$record_id."' AND student_flags_id='".$flag_id."' AND year='".$year."' AND term_id='".$term_id."'";
	$GLOBALS['dbh']->Execute($sql);
}
?>
