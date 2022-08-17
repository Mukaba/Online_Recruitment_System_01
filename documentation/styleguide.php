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
 | Filename: styleguide.php                                                     |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/

// This file was developed so people writing or modifying Mamook[tm] code would know
// the guidelines for keeping the output consistent.
include("../misc/constants.inc");
include("../misc/functions.inc");
?>
<html>
<head>
<title>
Mamook&#8482; styleguide reference
</title>
<link rel="stylesheet" href="../misc/css/colorgeneric.css" type="text/css">

</head>
<body bgcolor="WHITE">
<h1>Mamook&#8482; Styleguide</h1>
This styleguide was developed in order to maintain consistency for the output from Mamook&#8482;.
<h2>Titles</h2>
Every page should have a <b>main title</b>. The main title for the page should be in &lt;H3&gt; format and should start with the name of the main menu module to which the 
page belongs.  This should be followed by a space, then a hyphen (-), then another space, and finally by a word representing the particular page.  If the 
page is the first page you are sent to by clicking on a menu item/sub-item, then this last part of the title should be the name of the sub-menu item to 
which the page belongs.  Otherwise, the last part of the title may be the name of the sub-menu item or may be something more representative of the specific page. 
The main title should NOT have a colon (:) at the end.
<br />
<table border=2>
<tr><td>Main title example:</td><td>
<h3>Student Information - Advanced Search</h3>
</td></tr>
</table>
<br />
Some pages will have a <b>secondary title</b>. The secondary title provides more detailed information about the specific page. The secondary title could 
be used to indicate sub-groupings within the page. For example, "Current Jobs" and "Past Jobs" could be secondary titles if they were contained in the 
same page. The secondary title should be in &lt;H4&gt; format and should end in a colon (:). 
<table border=2>
<tr><td>Secondary title example:</td><td>
<h4>Current Jobs for Summer 2002:</h4>
</td></tr>
</table>
<br />
A <b>tertiary/informational title</b> can be used when there is additional information to present.  The tertiary/informational title has a label portion 
in <b>bold</b>, followed by a colon (:) and then the information.  If multiple tertiary/informational titles are listed together, a table should be set 
up in order to have the information content line up.  Both columns of this table should be left-justified.
<table border=2>
<tr><td>Tertiary/informational title example:</td><td>
<table><tr><td>
<b>Student Name:</b></td><td>Joe Smith</td></tr>
<tr><td><b>Student Number:</b></td><td>1234567</td></tr>
<tr><td><b>Job Code:</b></td><td>Mot-123
</td></tr></table>
</td></tr>
</table>
<br />
<h2>Page Layout</h2>
<ul>
<li><b>Text entry</b> pages have a <span class=row1>row1</span> coloured background with no border. Sub-sections within text entry pages can be grouped by 
providing a <span class=row0>row0</span> lighter coloured background within the darker background.
<li><b>View</b> pages have a transparent background.
<li><b>Instructions</b> within a coloured background should be in <b class=black>bold, class=black</b>.
<li>An <b>&lt;HR&gt;</b> should be applied after instructions within a coloured background, as appropriate.  
<li>An <b>&lt;HR&gt;</b> should be used to separate a form from its buttons.
<li><b>Buttons</b> on a text entry form should be within the coloured background, separated from the entry area by an &lt;HR&gt;
<li><b>Buttons</b> on a view page should be on a transparent background, with no &lt;HR&gt; separating them from the preceeding sections.
</ul>

<h2>Menu Breakdown</h2>
<ul>
<li><b>Menus across the top of the screen</b> offer a further breakdown of choices over the menu options available on the side menu.  For example, after clicking
on the <b>Employer Info</b> menu option, followed by <b>Edit</b>, three choices are displayed across the top: <b>Company</b>, <b>Division</b>, <b>Contact</b>.
<li><b>Quick Menus</b> (Pull-down menu displayed near the top of the screen with a green arrow) are used to move to different screens involving the record that is
<b>currently open</b>.  For example, if you are currently viewing a student, the quickmenu should provide options that allow you to view different screens for that
same student, such as <b>Edit</b>, <b>Set Actions</b>, <b>Apply Student to Job</b>, <b>View Student's Interviews</b>, etc.
</ul>

<h2>Notification</h2>
<?php
instruct("The \"instruct()\" function is used to provide instructions in cases where the instructions cannot go in a shaded input area on the screen.  eg. A page where you are selecting an item from a returned list. <b class=instruct>Bold text</b> should be in &lt;b class=instruct&gt; tags.");
?>
<?php
info("The \"info()\" function is used to provide grouped information within a view page.  eg. The presentation info within the modify sign-up page.");
?>
<?php
notify("The \"notify()\" function is used to provide confirmation. eg. \"Your e-mail has been sent successfully.\"");
?>
<?php
error("The \"error()\" function is used to indicate that there is a problem. eg. \"E-mail address invalid.  Please push the BACK button on your browser and try again.\"");
?>
<h2>Tables</h2>
<ul>
<li>Tables should have a <b>title-bar</b> that identifies the columns.
<li>Table titles should be <b>centered</b>.
<li>Table <b>headers</b> are in a <span class=rowgrey><b class=white>rowgrey</b></span> colour background with a &lt;b class=white&gt; font.
<li>Tables listing items should <b>alternate rows</b> of class <span class=row0d>row0d</span> and <span class=row1d>row1d</span>.
<li>Most tables listing items should be <b>two nested tables</b>.  The outer table has cellspacing=0, cellpadding=0, border=1.  The inner table has cellpadding=2, border=0.
<li>Both table headers and table items should have "&amp;nbsp;" before and after them to give <b>added horizontal space</b>.
</ul>
<b>Example of a table:</b>
<table cellspacing="0" cellpadding="0" border="1"><tr><td>
<table border="0" cellpadding="2">
<tr>
<td class="rowgrey" align='center'>&nbsp;<b class="white">Column 1</b>&nbsp;</td>
<td class="rowgrey" align='center'>&nbsp;<b class="white">Column 2</b>&nbsp;</td>
<td class="rowgrey" align='center'>&nbsp;<b class="white">Column 3</b>&nbsp;</td>
</tr>
<tr>
<?php
$rowclass=1;
echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
echo("Item 1.1</td>\n");
echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
echo("Item 2.1</td>\n");
echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
echo("Item 3.1</td>\n");
echo("</tr>\n");
$rowclass=0;
echo("<tr>\n");
echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
echo("Item 1.2</td>\n");
echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
echo("Item 2.2</td>\n");
echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
echo("Item 3.2</td>\n");
echo("</tr>\n");
$rowclass=1;
echo("<tr>\n");
echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
echo("Item 1.3</td>\n");
echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
echo("Item 2.3</td>\n");
echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
echo("Item 3.3</td>\n");
echo("</tr>\n");
?>
</table>
</td></tr></table>
<h2>Mail</h2>
All mail messages should use the "error()" function to display e-mail errors.  They should also include a second sentence: "<b>Please push the BACK button 
on your browser and try again.</b>"
<h2>Wording</h2>
<ul>
<li><b>Shortlist</b> is one word, both for the noun and the verb.
<li><b>Cover letter</b> is two words.
<li><b>Sign up</b> is two words when used as a verb, and has a hyphen (<b>sign-up</b>) when used as a noun or adjective.
<li><b>Log in</b> is two words when used as a verb, and is one word (<b>login</b>) when used as a noun or adjective.
<li><b>E-mail</b> has a hyphen. The 'M' is not capitalized.
<li><b>Co-op</b> and <b>Co-operative Education</b> have hyphens.
<li>When referring to browser buttons, use capital letters: <b>BACK button</b>
</ul>
<h2>Dates</h2>
Dates should be displayed using the following format: November 15, 2002 (3:20 pm) . The PHP function for this is: date("F j, Y (g:i a)"). The SQL function for
this is DATE_FORMAT(some_date_time, '%M %d, %Y (%l:%i %p)').
</body>
