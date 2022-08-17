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
 | Filename: editchoose.inc                                                     |
 +------------------------------------------------------------------------------+
 | Description: This form is called by edit.inc and is used to determine which  |
 | of the three options (jobinfo, date/time, students) the user would like to   |
 | edit, and calls the appropriate form.                                        |
 +------------------------------------------------------------------------------+

*/


?>

<?php
switch($level1)
{
	case "students":
	if ($userlevel == OFFICE)
	{
		include("interview/edit/edit_students.inc");
	} else
	{
		include("misc/loguser.inc");
	}
	break;
	
	case "timedate":
	if ($userlevel == OFFICE)
	{
		include("interview/edit/edit_datetime.inc");
	} else
	{
		include("misc/loguser.inc");
	}
	break;

	case "location":
	if ($userlevel == OFFICE)
	{
		include("interview/edit/edit_location.inc");
	}
	else
	{
		include("misc/loguser.inc");
	}
	break;

	case "jobinfo":
	default:
	if ($userlevel == OFFICE)
	{
		include("interview/edit/edit_jobinfo.inc");
	} else
	{
		include("misc/loguser.inc");
	}
	break;
} //switch

?>

