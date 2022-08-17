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
 | Filename: view_by_week.inc                                                   |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/

?>
<h3>Interviews - View by Week</h3>
<?php
if($Back){
	$row = $GLOBALS['dbh']->GetRow("SELECT DATE_SUB('" . $start_date . "', INTERVAL 1 DAY)");
	$start_date = $row[0];
}elseif($Forward){
	$row = $GLOBALS['dbh']->GetRow("SELECT DATE_ADD('" . $start_date . "', INTERVAL 1 DAY)");
	$start_date = $row[0];
}elseif($BackWeek){
	$row = $GLOBALS['dbh']->GetRow("SELECT DATE_SUB('" . $start_date . "', INTERVAL 7 DAY)");
	$start_date = $row[0];
}elseif($ForwardWeek){
	$row = $GLOBALS['dbh']->GetRow("SELECT DATE_ADD('" . $start_date . "', INTERVAL 7 DAY)");
	$start_date = $row[0];
}elseif($JumpWeeks){
	if ($JumpWeeks > 0) {
			$sql = "SELECT DATE_ADD('" . $start_date . "', INTERVAL ". $JumpWeeks ." DAY)";
			$row = $GLOBALS['dbh']->GetRow($sql);
			$start_date = $row[0];
	} elseif ($JumpWeeks < 0) {
			$JumpWeeks *= -1;
			$sql = "SELECT DATE_SUB('" . $start_date . "', INTERVAL ". $JumpWeeks ." DAY)";
			$row = $GLOBALS['dbh']->GetRow($sql);
			$start_date = $row[0];
	}

}

if ($Today || $start_date==""){
	$row = $GLOBALS['dbh']->GetRow("SELECT CURRENT_DATE()"); 
	$start_date = $row[0];
}

include ("interview/view/list/view_by_week_main.inc");
