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
 | Filename: confirm_paging.php                                                 |
 +------------------------------------------------------------------------------+
 | Description: This script is used to confirm that the user wants to go to     |
 | another page without saving their changes.                                   |
 +------------------------------------------------------------------------------+

*/


?>

<script type="text/javascript" language="javascript">
<!--javascript

function unfocus()
{
        self.close();
}

function resizeOuterTo() 
{
    var linuxPattern = /X11/;
    var pcPattern = /Windows/;
    var macPattern = /PowerPC/;

    var result;
    result=linuxPattern.exec(navigator.appVersion);
    if (result != null)
    {
        // Linux system.
        window.resizeTo(500, 200);
    } 
    else
    {
        result = pcPattern.exec(navigator.appVersion);
        if (result != null)
        {
            // PC system
            window.resizeTo(500, 200);
        }
        else if (parseInt(navigator.appVersion)>3)
        {
            // Mac System
        }
    }
}

function cancelSubmission()
{
    // Just let them return to the previous page.
    self.close();
}

function proceedNoSaving()
{
    var contact_flag = window.opener.document.form2.contact_flag[window.opener.document.form2.contact_flag.selectedIndex].value;
    window.opener.document.paging_form.contact_flag.value = contact_flag;
    window.opener.document.paging_form.submit();
    self.close();
}

function saveAndProceed()
{
    var saveChanges = "Save Changes";
    var startRow = window.opener.document.paging_form.start_row[window.opener.document.paging_form.start_row.selectedIndex].value;
    window.opener.document.form2.start_row.value = startRow;

    // This is inelegant, and needed because I foolishly named the control variable continue in my HTML forms.
    for (var i = 0; i < window.opener.document.form2.elements.length; i++)
    {
        var e = window.opener.document.form2.elements[i];
        if (e.type == 'hidden' && e.name == 'continue')
        {
            e.value = saveChanges;
        }
    }
    window.opener.document.form2.submit();
    self.close();
}

//-->

</script>

<?php

// Define a constant that leads back to the top directory.
DEFINE("DIR_PATH", "../../../");

include(DIR_PATH . 'misc/wwwclient.inc');
include(DIR_PATH . 'misc/release_constants.inc');
include(DIR_PATH . 'misc/constants.inc');
include(PATH_ADODB . 'adodb.inc.php');
include(DIR_PATH . 'misc/db.inc');
include(DIR_PATH . 'misc/functions.inc');
$client = new wwwclient();

if ($client->platform == WIN)
{
	echo("<link rel='stylesheet' href='" . DIR_PATH . "misc/css/font-win.css' type='text/css'>");
}
elseif ($client->browser == IE)
{
	echo("<link rel='stylesheet' href='" . DIR_PATH . "misc/css/font-nonwin-ie.css' type='text/css'>");
}
else
{
	echo("<link rel='stylesheet' href='" . DIR_PATH . "misc/css/font-nonwin.css' type='text/css'>");
}

echo("<link rel='stylesheet' href='" . DIR_PATH . "misc/css/color" . $_SESSION['SESSION_colorscheme'] . ".css' type='text/css'>");

echo("<body bgcolor='#FFFFFF' hspace='0' vspace='0' marginwidth='0' marginheight='0' leftmargin='0' rightmargin='0' topmargin='0' bottommargin='0' frameborder='0' border='0' frameborder='no' background='".$GLOBALS['colorscheme']['bg'].">");
echo("<form method='post' action=''>");

echo("<center>");
echo("<table cellpadding='5' cellspacing='5'>");

echo("<tr align='center'>");
    echo("<td>");
    notify("You have made changes to the flags, but have not yet saved those changes.  Please select one of the three options below.</h4>");
    echo("</td>");
echo("</tr>");

echo("<tr>");
    echo("<td>&nbsp;</td>");
echo("</tr>");

echo("<tr align='center'>");
    echo("<td nowrap='nowrap'>");
    echo("<input type='button' value='Cancel' onclick='cancelSubmission();' />");
    echo("<input type='button' value='Proceed Without Saving' onclick='proceedNoSaving();' />");
    echo("<input type='button' value='Save and Proceed' onclick='saveAndProceed();' />");
    echo("</td>");
echo("</tr>");

echo("</table>");
echo("</center>");

?>
<script type="text/javascript" language="javascript">
<!--javascript
resizeOuterTo();
//-->
</script>
<?php

echo("</form>");
echo("</body>");

?>
