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
 | Filename: html_editor.inc                                                    |
 +------------------------------------------------------------------------------+
 | Description: Brings up a form where the user can edit their coverletter.     |
 +------------------------------------------------------------------------------+

*/

$error = "";

if ($save_error)
{
    error($save_error);
}

//we're actually editing something that already exists
elseif ($coverletter_id)
{
    // look and check for the student's coverletter
    $sql = ("
        SELECT distinct coverletter_id, job_id, type_id, letter, student_number,
        DATE_FORMAT(last_modified, '%M %d, %Y %H:%i:%S') as last_modified
        FROM coverletter
        WHERE coverletter_id='$coverletter_id' AND student_number='".addslashes(stripslashes($student_number))."'
        ");

    $result = $GLOBALS['dbh']->Execute ($sql);
    if ($result->RecordCount() <= 0) 
    {
        $error = "No cover letter found.";
    }
    
    else 
    {
        $row = $result->FetchRow();
        $type_id = $row["type_id"];
        $letter = $row["letter"];

        if ($client->browser != IE)
        {
            $letter = ereg_replace("&amp", "&amp;amp",$letter);  // preserves &amp; when editing
            $letter = ereg_replace("&nbsp","&amp;nbsp",$letter); // preserves &nbsp;
        }
        
        $last_modified = $row["last_modified"];
        $job_id = $row["job_id"];
    }

    // to be placed at the top
    $job_code = "";
    if ($type_id == TEMPLATE)
    {
        $job_code = "Template";
    }
    
    elseif($type_id == GENERIC)
    {
        $job_code = "Generic";
    }
    else
    {
        $sql = "SELECT job_code FROM job_info WHERE job_id = '" . addslashes($job_id) . "'";
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();
        $job_code = $row["job_code"];
    }

} // done stuff if editing



if ($error != "" )
{
    error($error);
    include( "coverletters/student/edit_main.inc" );

}
else 
{
 echo ("<form method='post' action='$PHP_SELF'>");
    echo ("<table width='80%' class='row1' cellpadding='0' cellspacing='0' border='0'>
        <tr>
        <td>");
        
    echo ("<table width='100%' class='row1' cellpadding='3' cellspacing='4' border='0'>
        <tr>");
    
        echo ("<td>");
        echo ("<h4>Cover Letter HTML Editor:</h4>");
    if($application_id)
    {
       // echo ("<form method='post' action='$PHP_SELF'>");
        echo ("<input type='hidden' name='page' value='save coverletter' />");
        echo ("<input type='hidden' name='select' value='applications' />");
        echo ("<input type='hidden' name='application_id' value='$application_id' />");
        echo ("<input Type='hidden' Name='coverletter_id' value='$coverletter_id' />");
        echo ("<input type='hidden' name='option' value='edit_delete' />");
        echo ("<input type='hidden' name='edit' value=' Edit ' />");
        echo ("<input type='hidden' name='PDF' value='1' />");
    }
    else
    {
      //  echo ("<form method='post' action='$PHP_SELF'>");
        echo ("<input type='hidden' name='page' value='save' />");
        echo ("<input type='hidden' name='select' value='$select' />");
        echo ("<input type='hidden' name='type_id' value='$type_id' />");
        echo ("<input Type='hidden' Name='coverletter_id' value='$coverletter_id' />");
        echo ("<input type='hidden' name='PDF' value='1' />");
    }


        echo ("</td>");
    echo ("</tr>");
    

    if ($job_code && ($type_id == SPECIFIC)) 
    {

        echo ("<tr>");
            echo ("<td>");
            echo ("<b class='black'>Job Code: $job_code</b>");
            echo ("</td>");
        echo ("</tr>");
    }
/*
    echo ("<tr>");
        echo ("<td>");
        echo ("Last Modified: ". (($last_modified == "") ? "Never" : $last_modified));
        echo ("</td>");
    echo ("</tr>");
*/
    echo ("<tr>");
        echo ("<td>");
        echo ("Please enter the HTML code for your coverletter below.");
        echo ("<hr />");
        echo ("</td>");
    echo ("</tr>");

    echo ("<tr>");
        echo ("<td align='center'>");
        echo ("<textarea name='letter' cols='90' rows='30' wrap>$letter</textarea>");
        echo ("</td>");
    echo ("</tr>");


    echo ("<tr>");
        echo ("<td align='center'>");
            echo ("<hr />");
            echo ("<input type='submit' value='Save Cover Letter' />");
            echo ("</td>");
    echo ("</tr>");
    echo ("</table>");

    echo ("</td>
        </tr>
        </table>");
    echo ("</form>");
}

include ('misc/HTMLtoPDFInfo/HTMLtoPDFInfo.inc');
?>
