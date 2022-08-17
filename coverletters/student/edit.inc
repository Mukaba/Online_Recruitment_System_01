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
 | Description: Brings up a form where the user can edit their coverletter.     |
 +------------------------------------------------------------------------------+

*/

$error = "";

if ($save_error)
{
    error($save_error);
}

//we're actually editing something that already exists
elseif ($page != "Add Job" && $page != "Add Template" && $page != "Add Generic")
{

    // look and check for the student's coverletter
    $sql = ("
        SELECT distinct coverletter_id, job_id, type_id, letter, student_number,
        DATE_FORMAT(last_modified, '%M %d, %Y %H:%i:%S') as last_modified
        FROM coverletter
        WHERE coverletter_id='".$coverletter_id."' AND student_number='".addslashes(stripslashes($student_number))."'
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

        // IE displays special HTML entities fine in its textarea boxes. 
        if ($client->browser != IE)
        {
            $letter = ereg_replace("&amp", "&amp;amp",$letter);  // preserves &amp; when editing
            $letter = ereg_replace("&nbsp","&amp;nbsp",$letter);   // preserves &nbsp;
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
    ?>
    <form method="post" action="<?php echo $PHP_SELF?>">
    <input type="hidden" name="select" value="<?php echo($select); ?>" />
    echo("<input type="hidden" name="type_id" value="<?php echo $type_id?>" />");
    <input Type="hidden" Name="coverletter_id" Value="<?php echo $coverletter_id?>" />
    <input type="hidden" name="PDF" value="1" />
    <table border="0" width="100%" cellpadding="0" cellspacing="2">
      <tr>
        <td><h3>Edit Cover Letter</h3></td>
      </tr>

      <tr>
        <td><b class="black">Job Code: <?php echo $job_code?></b></td>
      </tr>

      <tr>
        <td>Last Modified: <?php echo (($last_modified == "") ? "Never" : $last_modified)?></td>
      </tr>

      <tr>
        <td><hr width="100%" /></td>
      </tr>
        <tr>
        <td><h4>Edit HTML:</h4>
        <table width="100%" cellpadding="5" cellspacing="0" border="0" class="row1">
    <?php if ($last_modified == "") { ?>
    <tr><td>

        </td></tr>
        <?php } ?>
            <tr>
              <td align="center"><textarea name="letter" cols="75" rows="30" wrap><?php echo ($letter) ?></textarea></td>
            </tr>
            <tr>
              <td>
                <table border="0" width="100%">
                  <tr>
                    <td align="right" width="50%"><input type="submit" name="page" value="Save Cover Letter" /></td>
                    <td align="left" width="50%"><input type="reset" /></td>
                  </tr>
                </table>
              </td>
            </tr>
            </table>
            <br />
            <hr width="100%" />
            </td>
          </tr>
            <?php
            /* DISABLED FOR JANUARY 2002
          <tr>
            <td><h4>Update with File:</h4>
            <table width="100%" class="row1" cellspacing="0" cellpadding="5" border="0">
              <tr>
                <td>File: <input type="FILE" name="letter_file" size="40" /></td>
              </tr>
              <tr>
                 <td><input type="submit" value="Submit File" name="page" /></td>
              </tr>
            </table>
            </td>
          </tr>
            */?>
        </table>
        </form>
    <?php
    }



// IS this really needed????
function checkValidJob($job_id, $type_id)
{
    $error = "";
    //if job_id = 0 then type must either by 2 or 3 otherwise type must be 1
    if ($job_id == 0)
    {
        if ($type_id != TEMPLATE && $type_id != GENERIC)
        {
            $error = "Invalid Letter Type";
        }
    
        $sql = ("
            SELECT type_id
            FROM coverletter_type
            WHERE type_id = '" . addslashes(stripslashes($type_id)) . "'
            ");
        
        $result = $GLOBALS['dbh']->Execute($sql);
        if ( $result->RecordCount() <= 0)
        {
            $error = "Invalid Letter Type";
        }
    }
    
    else
    {
        if ($type_id != SPECIFIC)
        {
            $error = "Invalid Letter Type";
        }
        
        $sql = ("
            SELECT job_id
            FROM job_info
            WHERE job_id = '" . addslashes(stripslashes($job_id)) . "'
            ");

        $result = $GLOBALS['dbh']->Execute($sql);
        if ( $result->RecordCount() <= 0 )
        {
            $error = "Invalid Job Id";
        }
    }
    return $error;
}

?>
