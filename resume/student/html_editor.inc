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
 | Description: HTML editor for resumes                                         |
 +------------------------------------------------------------------------------+

*/

$resume = "";
if ($use_template == "on") 
{
    // retrieve template from database and use that

    $sql = "select resume,name from resume where student_number=\"$student_number\" and type_id=\"".TEMPLATE."\"";
    $response = $GLOBALS['dbh']->Execute ($sql);

    if (!$response->RecordCount()) 
    {

        // IMPOSSIBLES I TELL YOU
        error ("Resume template not found.");
    }

    else 
    {
        $row = $response->FetchRow();
        $resume = "<!-- Generated on ".date("Y-m-d H:i:s")." from the resume template\n     ".
            "by the ".BRAND_NAME." resume generator -->\n".$row["resume"];
    }
}

if (isset ($resume_id)) 
{
    $sql = ("
        SELECT resume,name,type_id 
        FROM resume 
        WHERE resume_id='".$resume_id."' AND student_number='".$student_number."'
        ");
    $response = $GLOBALS['dbh']->Execute ($sql);
    
    if ($response->RecordCount()) 
    {
        $row = $response->FetchRow();
        $resume = $row["resume"];

        // IE's textarea displays special HTML entities fine. 
        if ($client->browser != IE)
        {
            $resume = ereg_replace("&amp", "&amp;amp",$resume);   //preserves &amp;
            $resume = ereg_replace("&nbsp","&amp;nbsp",$resume);  //preserves &nbsp;
        }
    
        $name = $row["name"];
        $type_id = $row["type_id"];
    }

    else 
    {
        error ("invalid resume ID");
        $dont_show_editor = 1;
    }
}

if (!$dont_show_editor) 
{
    if (isset($resume_name))   //We need this check because $resume_name is only set when adding a resume not editing
    { 
        $name = $resume_name;  //Set $name to default name (e.g. Specific resume's name set to 'Specific')
    }

    echo ("<form method='post' action='$PHP_SELF'>");      
    echo ("<table width='80%' class='row1' cellpadding='0' cellspacing='0' border='0'>
        <tr>
        <td>");
        
    echo ("<table width='100%' class='row1' cellpadding='3' cellspacing='4' border='0'>
        <tr>");
    
        echo ("<td>");
        echo ("<h4>Resume HTML Editor". ($name ? " ($name)" : "") .":</h4>");
        echo ("</td>");
    echo ("</tr>");
    
    echo ("<tr>");
        echo ("<td>");
        echo ("Please enter the HTML code for your resume below.");
        echo ("<hr />");
        echo ("</td>");
    echo ("</tr>");

    echo ("<tr>");
        echo ("<td align='center'>");
        echo ("<textarea rows='30' cols='90' name='resume'>$resume</textarea>");
        echo ("</td>");
    echo ("</tr>");

    echo ("<tr>");
        echo ("<td align='center'>");
        echo ("<hr />");
        
        echo ("<input type='hidden' name='type_id' value='$type_id' />");
        echo ("<input type='hidden' name='select' value='$select' />");
        echo ("<input type='hidden' name='name' value='".htmlentities($name, ENT_QUOTES)."' />");
        echo ("<input type='hidden' name='PDF' value='1' />");
    
        if (isset ($resume_id)) 
        {
            echo ("<input type='hidden' name='resume_id' value='$resume_id' />");
        }

        echo ("<input type='submit' name='page' value='Save Resume' />");
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
