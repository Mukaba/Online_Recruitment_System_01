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
 | Filename: set.inc                                                            |
 +------------------------------------------------------------------------------+
 | Description: This file is the start of setting contact flags.  It decides    |
 | whether the user wants to set actions or flags for contacts.                 |
 +------------------------------------------------------------------------------+

*/


include('contact/set/set_header.inc');

switch($level1)
{
case "flags":
	if ($userlevel == OFFICE || $userlevel == ADMINISTRATOR || $userlevel == TRAFFICDIRECTOR)
	{
		$sql = ("
			SELECT DISTINCT *
			FROM department_contact_flags
			WHERE department_id='" . $auth->department . "'
			");
		$result = $GLOBALS['dbh']->Execute($sql);

		if (!$result->RecordCount())
		{
			$msg .= ("Your department has not yet designated any flags that it wishes to be able to edit for contacts, and thus cannot");
			$msg .= (" currently set any flags.");
			error($msg);
			include('contact/set/action/action_header.inc');
		}
		else
		{
			include('contact/set/flag/flag_header.inc');
		}
	}
	else
	{
		include('misc/loguser.inc');
	}
	break;
case "actions":
default:
	if ($userlevel == OFFICE || $userlevel == ADMINISTRATOR || $userlevel == TRAFFICDIRECTOR)
	{
		include('contact/set/action/action_header.inc');
	}
	else
	{
		include('misc/loguser.inc');
	}
	break;
} //switch

?>
