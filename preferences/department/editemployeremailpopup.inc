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
 | Filename: EditEmployerEmailPopup.php                                         |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/


/*
 EditEmployerEmailPopup
 Edit the message that gets sent to employers for the first they need info or login or whatever.
*/
$department_id = $auth->department;

$done = false;
if ($choice == 'Cancel'){
	$done = true;
} elseif ($choice == 'Save') {
    $binsert = "UPDATE department SET application_employer_body = '". addslashes(stripslashes($_REQUEST['body'])) ."'
		        WHERE department_id = '$department_id'";
	if ($GLOBALS['dbh']->Execute ($binsert)){
		$done = true;
    }
} else {
	$bresult = $GLOBALS['dbh']->Execute ("SELECT application_employer_body FROM department WHERE department_id = '$department_id'");
	if ($brow = $bresult->FetchRow()){
		$emp_body = stripslashes ($brow["application_employer_body"]);
    }
}

if ($done) {
?>
<script type='text/javascript' language='javascript'>
//<![CDATA[ 
    self.close();
//]]>
</script>
<?php
}
?>
<script ='text/javascript'  language='javascript'>
//<![CDATA[

var w = window.outerwidth;
var h = window.outerheight;
var pattern = /X11/;
var result;
result=pattern.exec(navigator.appVersion); 
if (result != null) 
	window.resizeTo(670, 530);
if (parseInt(navigator.appVersion)>3)
	if (navigator.appName=="Netscape")
		window.resizeTo (670, 530);
//]]>
</script>

<?php
echo ("<form name='form1' action='mamook.php?select=editemployeremailpopup&no_headers=1' method='post'>\n");


// 'page table'
echo ("<table border='0' width='100%' height='100%' align='center' valign='middle'>\n");
echo ("<tr align='center'><td align='center'>");

	echo ("<table name='mainTable' border='0' cellpadding='10' cellspacing='0' class='row1' width='535'>");
?>		
		<tr>
			<td align='center' colspan='2' nowrap>
			<b class='black'>Edit Employer E-mail Message for Job Applications</b>
			</td>
		</tr>
		
		<tr>
			<td colspan='2'><textarea cols='100' rows='30' name='body'><?php echo ($emp_body); ?></textarea></td>
		</tr>

		<tr>
			<td align='center' colspan='2'>
			<hr />
			<input type='submit' name='choice' value='Save' />
			&nbsp; &nbsp; &nbsp;
			<input type='submit' name='choice' value='Cancel' />
			</td>
		</tr>
	</table>
</td>
</tr>
</table></form>
