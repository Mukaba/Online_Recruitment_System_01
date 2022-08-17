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
 | Filename: delete.inc                                                         |
 +------------------------------------------------------------------------------+
 | Description: Ensures the selected coverletter to delete is valid and NOT     |
 | ACTIVE.  Prompts the user for confirmation to delete the coverletter and     |
 | then deletes the coverletter from the system                                 |
 +------------------------------------------------------------------------------+

*/

$error = "";
if ($confirm == "Yes"){
    if ($job_id == 0 && ($type_id != GENERIC && $type_id != TEMPLATE) || ($job_id != 0 && $type_id != SPECIFIC)){
        $error = "Invalid Cover Letter.";
    }else{
        $sql = "SELECT count(*) as c_count FROM coverletter WHERE student_number = '" . addslashes($student_number) . "' AND job_id = '" . addslashes($job_id) . "' AND type_id = '" . addslashes($type_id) . "'";
        $result = $GLOBALS['dbh']->Execute($sql);
        $row = $result->FetchRow();
        if ($row["c_count"] <= 0){
            $error = "You do not have a cover letter for the selected job.";
        }
    }
    if ($error != ""){
        error ($error);
    }else{
    
        $sql = "DELETE 
            FROM coverletter 
            WHERE student_number = '$student_number' AND job_id = " . addslashes(stripslashes($job_id)) . " 
                AND type_id = " . addslashes(stripslashes($type_id)) . " AND (status_id != '" . ACTIVE . "' or status_id is null)";
        $result = $GLOBALS['dbh']->Execute( $sql );
        if (!$result || $GLOBALS['dbh']->Affected_Rows() == 0){
            error ( "Can not delete cover letter." );
        }else{
            if ($type_id == GENERIC){
                $msg = "Your generic cover letter has been deleted from the system.";
            }elseif($type_id == TEMPLATE){
                $msg = "Your template cover letter has been deleted from the system.";
            }elseif($type_id = SPECIFIC){
                $result=$GLOBALS['dbh']->Execute("SELECT job_code FROM job_info where job_id = '" . addslashes($job_id) . "'");
                $row = $result->FetchRow();
                $msg = "<p>Your cover letter for job " . $row["job_code"] . " has been deleted from the system.</p>";
            }
            echo "<table border='0' cellpadding='5' class='backgrey'><tr><td>" . $msg . "</td></tr></table>\n";
        }
    }
    include( "coverletters/student/delete_main.inc" );
}else{
    
    if ($job_id == 0 && ($type_id != GENERIC && $type_id != TEMPLATE) || ($job_id != 0 && $type_id != SPECIFIC)){
        $error = "Invalid Cover Letter.";
    }else{
        $sql = "SELECT status_id
            FROM coverletter 
            WHERE student_number = '" . addslashes($student_number) . "' AND job_id = '" . addslashes($job_id) . "'
                AND type_id = '" . addslashes($type_id) . "' AND (status_id != '" . addslashes(ACTIVE) . "' or status_id is null)";
        $result = $GLOBALS['dbh']->Execute($sql);
        if($result->RecordCount() == 0){
            $error = ("You can not delete a cover letter once it has been sent to the employer.");
        }else{
            if ($type_id == GENERIC){
                $msg = "Are you sure you want to delete your generic cover letter?";
            }elseif($type_id == TEMPLATE){
                $msg = "Are you sure you want to delete your template cover letter?";
            }elseif ($type_id == SPECIFIC){
                $sql = "SELECT count(*) as c_count FROM coverletter WHERE student_number = '" . addslashes($student_number) . "' AND job_id = '" . addslashes($job_id) . "' AND type_id = '" . addslashes($type_id) . "'";
                $result = $GLOBALS['dbh']->Execute($sql);
                $row = $result->FetchRow();
                if ($row["c_count"] > 0){
                    $sql = "SELECT job_code
                        FROM job_info
                        WHERE job_id = '" . addslashes($job_id) . "'";
        
                        $result = $GLOBALS['dbh']->Execute($sql);
                        $row = $result->FetchRow();
                        $msg = "Are you sure you want to delete your cover letter for job " .  $row["job_code"] . "?";
                }else{
                    $error = "You do not have a cover letter letter for the selected job.";
                }
            }else{
                 $error  = "Invalid type.";
            }
        }
    }
    if ($error != ""){
        error ($error);
        include ("coverletters/student/delete_main.inc");
    }else{
        ?>

<table class='backgrey' cellpadding='5' width='100%'>
  <tr>
    <td>
      <h4>Delete Cover Letter Confirmation:</h4>
      <p>
      <?php echo $msg?>
      </p>
      <table cellpadding="0" cellspacing="0" border="0" align="center">
        <tr>
      <td>
          <table width="100%" border="0" cellpadding="0" cellspacing="0">
            <tr>
         <td>
            <form action="<?php echo $PHP_SELF?>" method="post">
            <input type="hidden" name="select" value="coverletter" />
            <input type="hidden" name="job_id" value="<?php echo $job_id?>" />
            <input type="hidden" name="type_id" value="<?php echo $type_id?>" />
            <input type="hidden" name="page" value="Delete" />
            <input type="submit" name="confirm" value="Yes" />
            </form>
          </td>
          <td>
            <form action="<?php echo $PHP_SELF?>" method="post">
            <input type="hidden" name="select" value="coverletter" />
            <input type="hidden" name="page" value="view" />
            <input type="submit" name="confirm" value="No" />
            </form>
          </td>
        </tr>
          </table>
          </td>
    </tr>
      </table>
    </td>
  </tr>
</table>
<?php
}}
?>

