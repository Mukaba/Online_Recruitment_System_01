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
 | Filename: history_header.inc                                                 |
 +------------------------------------------------------------------------------+
 | Description: This is the file that holds the header information for          |
 | view_student.inc. It contains the menu options across the top of the         |
 | screen.                                                                      |
 +------------------------------------------------------------------------------+

*/

echo("&nbsp;</td><td align='left'><img src= '".$GLOBALS['colorscheme']['topbar_right']."' alt='' /></td></tr></table>");

echo("<table width=100%>");
echo("<tr>");
switch ($continue)
{
    default:
    $page_title = "Student Information - Placement History";
}

echo("<td><h3 align='left'>".$page_title."</h3></td>\n");
// Display the quickmenu
echo("<td align='right'>");
if (trim($continue) && $record_id)
{
    include('history/history_quickmenu.inc');
}
echo("</td>\n");
echo("</tr>");
echo("</table>");

// This is used to notify if a e-mail was sent properly, or any sort of confirmation.
if ($msg)
{
    notify($msg);
}

?>
