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
 | Filename: activate_applications.inc                                          |
 +------------------------------------------------------------------------------+
 | Description: This file takes the $selected_app_id array (set in the file     |
 | that calls this one) and makes all of the selected students available.       |
 +------------------------------------------------------------------------------+

*/

for ($i = 0; $i < sizeof($selected_app_id); $i++)
{

	$checkSQL = $GLOBALS['dbh']->Execute ("select app_status from applications
				where application_id = '".$selected_app_id[$i]."'");
	$row = $checkSQL->FetchRow();

	$prev_status = $row["app_status"];

	// only update if they're set as applied (so not deleted)
	if (($prev_status == APP_PAPER_APPLIED) || ($prev_status == APP_APPLIED_ONLINE)) {
	
		$new_status = (($prev_status == APP_PAPER_APPLIED) ? APP_PAPER_SENT : APP_ACTIVATED_ONLINE);

		$sql = "UPDATE applications
			SET app_status='$new_status'
			WHERE application_id = '" . $selected_app_id[$i] . "'";
		$GLOBALS['dbh']->Execute($sql);
	}
}

notify("Selected students have been successfully made available.");
?>
