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
 | Filename: edit.inc                                                           |
 +------------------------------------------------------------------------------+
 | Description: Allows the user to edit the coverletter selected.               |
 +------------------------------------------------------------------------------+

*/

/*
if ($job_position != ""){
	$job_id = $job_id[$job_position];
	$type_id = $type_id[$job_position];
}elseif (is_array($job_id)){
	$job_id = "";
	$type_id = "";
}
*
if ($job_id == "" || $student_num == ""){
	error( "Please Select a Student and Job" );
	include( "coverletters/admin/view_main.inc" );
}elseif($use_generic == "on"){
	if ($job_id != 0){
		$type_id = SPECIFIC;
	}
	include( "coverletters/save.inc" );
	include( "coverletters/admin/view_results.inc" );
}else{
	$sql = "SELECT job_code 
		FROM job_info 
		WHERE job_id = " . addslashes(stripslashes($job_id));
    $result = $GLOBALS['dbh']->Execute($sql);
    $row = $result->FetchRow();

	$job_code = $row["job_code"];
	
	if ($job_id != "0" && $type_id == ""){
		$type_id = SPECIFIC;
	}
*/

if ($coverletter_id)
{
	$sql = "SELECT c.student_number, c.coverletter_id, ct.description as type, ji.job_code, 
        c.letter, DATE_FORMAT(c.last_modified, '%M %d, %Y (%l:%i %p)') as last_modified 
		FROM coverletter as c
        LEFT JOIN applications as a ON a.coverletter_id=c.coverletter_id
        LEFT JOIN job_info as ji ON a.job_id=ji.job_id
        LEFT JOIN coverletter_type as ct on c.type_id=ct.type_id
		WHERE c.coverletter_id = ".addslashes(stripslashes($coverletter_id));

	$result = $GLOBALS['dbh']->Execute( $sql );

	$row =  $result->FetchRow();

    echo("<table width=100%>");
    echo("<tr>");
    echo("<td><h3 align='left'>Student Information - Edit Cover Letter</h3></td>\n");

    // Display the quickmenu
    echo("<td align='right'>");
    if ($record_id)
    {
        include('student_info/student_quickmenu.inc');
    }
    echo("</td>\n");
    echo("</tr>");
    echo("</table>");
        $letter = $row["letter"];

        // IE browsers do not require this find and replace statement. It automatically preserves &nbsp; characters. 
        if ($client->browser != IE)
        {
            $letter = ereg_replace("&amp", "&amp;amp",$letter);    // preserves &amp; when editing
            $letter = ereg_replace("&nbsp","&amp;nbsp",$letter);   // preserves &nbsp;
        }
        $last_modified = $row["last_modified"];
        $student_number = $row["student_number"];
        $job_code = $row["job_code"];
        $type = $row["type"];

        ?>
            <b>Student:</b> <?php echo getStudentName($student_number)?><br />
            <b>Student Number:</b> <?php echo $student_number?><br />
<?php
            if ($job_code)
            {
                echo("<b>Job Code:</b> $job_code<br />");
            } else {
                echo("<b>Coverletter Type:</b> $type<br />");
            }
?>
            <b>Last Modified:</b> <?php echo (($last_modified == "") ? "&nbsp;" : $last_modified)?><br />
            <table border="0" width="100%" cellspacing="0" cellpadding="5" />
            <hr width="100%" />
            <tr>
            <td>
            <form method="post" action="<?php echo $PHP_SELF?>" enctype="multipart/form-data">
            <input type="hidden" name="select" value="coverletter" />
            <input type="hidden" name="department_id" value="<?php echo $department_id?>" />
            <input type="hidden" name="PDF" value="1" />
            <input type="hidden" name="coverletter_id" value="<?php echo $coverletter_id?>" />
            <input type="hidden" name="student_num" value="<?php echo $student_num?>" />
            <input type="hidden" value="<?php echo($term)?>" name="term" />
            <input type="hidden" value="<?php echo($year)?>" name="year" />
            <input type="hidden" value="<?php echo($term_year_submit)?>" name="term_year_submit" />

            <h4>Edit HTML:</h4>
            <table border="0" width="100%" class="row1" cellpadding="5" cellspacing="0">
            <tr>
              <td align="center"><textarea name="letter" cols="80" rows="30"><?php echo $letter?></textarea></td>
            </tr>
            <tr>
              <td>
                <table width="100%" border="0" cellspacing="0" cellpadding="3">
                  <tr>
                    <td width="50%" align="right"><input type="submit" value="Save Cover Letter" name="page" />&nbsp;&nbsp;</td>
                    <td width="50%" align="left">&nbsp;&nbsp;<input type="reset" value="Revert" /></td>
                  </tr>
                </table>
              </td>
            </tr>
              </table>
              </form>
            
          
        <?php
        /*
              <tr>
                <td><hr width="100%"><h4>Submit File:</h4>
                  <table border="0" width="100%" cellspacing="0" cellpadding="3" class="row1">
                    <tr>
                  <td>File:&nbsp;<input type="FILE" size="30" name="letter_file"></td>
                </tr>
                <tr>
                  <td><input type="SUBMIT" name="page" value="Submit File"></td>
                </tr>
                  </table>
                </td>
              </tr>
        */
        ?>
          
          </tr></td></table> 
        <?php


//}
} 


else 
{
    error("Please choose a specific cover letter.");
    unset( $PDF );
}
?>
