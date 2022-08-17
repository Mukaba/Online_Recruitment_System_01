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
 | Filename: date.inc                                                           |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/


// filename: date.inc

	if ($cal_date == "") { $cal_date = Date("Y-m-d"); } // default to today

?>
<form action="<?php echo($PHP_SELF . "&amp;select=room_booking") ?>" method="post">

<table>
	<tr>
		<td rowspan="2">
<?php

   include("misc/calendar.inc");
   calendar($cal_year, $cal_month, $cal_date, $cal_submit, false);
?>
		</td>
		<td rowspan="2">&nbsp;</td>
		<td>Building:</td>
	</tr>
	<tr>
		<td valign="top">
<?php

/*
$result = $GLOBALS['dbh']->Execute("
	SELECT building_id, building_code
	FROM building
	ORDER BY building_code");
*/
$result = $GLOBALS['dbh']->Execute("
	SELECT DISTINCT a.building_id, a.building_code
	FROM building AS a, room AS b
	WHERE a.building_id=b.building_id
	ORDER BY a.building_code");

global $auth;
$result_default = $GLOBALS['dbh']->Execute("
	SELECT default_building_id
	FROM department
	WHERE department_id='" . addslashes($auth->department) . "'");
if ($result_default && $row = $result_default->FetchRow())
{
	$default_id = $row["default_building_id"];
}

$size = $result->RecordCount();
if ($size > 12) { $size = 12; }
?>
		<select name="building_id" size="<?php echo($size) ?>">
<?php


while ($row = $result->FetchRow())
{
	$id = $row[building_id];
	echo("<option value='" . $id . "'" . (($id == $default_id) ? " selected='selected'" : "") . ">" . $row["building_code"] . "</option>\n");
}
?>
		</select>
		</td>
	</tr>
	<tr><td colspan="2" align="center"><input type="submit" value="Schedule" /></td></tr>
</table>
</form>
