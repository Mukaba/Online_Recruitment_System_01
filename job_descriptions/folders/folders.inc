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
 | Filename: folders.inc                                                        |
 +------------------------------------------------------------------------------+
 | Description: This is file manges job folders of a student                    |
 +------------------------------------------------------------------------------+

*/
?>
</td><td align="left"><img src="<?php echo $GLOBALS['colorscheme']['topbar_right']; ?>" alt="" /></td></tr></table>

<?php
include_once('job_descriptions/folders/functions.inc');
echo ("<h3>Manage Folders</h3>");
// First time the user has come to this page.
if($continue != "Rename" && $continue != "Empty Folder" && $continue != "Change" && $continue != "Remove Closed Jobs")
{
    echo ("<br />");
    instruct("Please select the folder you wish to manage, and then choose one of the options below.");
    echo ("<br />");
}
if($continue == "Empty Folder")
{
    if($folder_id)
    {
        $sql = ("
                DELETE FROM student_job_folder_join
                WHERE folder_id = '".$folder_id."'
                AND student_number = '".$student_number."'
                ");
        $result = $GLOBALS['dbh']->Execute($sql);
        $msg = (getFolderName($folder_id,$student_number)." folder has been emptied.  All the jobs that were in this folder can still be viewed by selecting ");
        $msg .= ("\"Any\" from the folder pulldown box.");
        notify($msg);
    }
    else
    {
        error("You must select a folder to empty.");
    }
    $show = true;
}
if($continue == "Remove Closed Jobs")
{
    if($folder_id)
    {
        $sql = ("
                SELECT sjfj.job_id 
                FROM job_info as a 
                INNER JOIN student_job_folder_join AS sjfj ON sjfj.job_id=a.job_id AND sjfj.student_number = '".$student_number."' 
                AND sjfj.folder_id='".$folder_id."' 
                WHERE (a.status=" . CLOSED . " OR (a.status=" . CANCELLED . " AND a.student_status='" . STUDENT_NOT_HIRING . "'))
                ");
        $result = $GLOBALS['dbh']->Execute($sql);
        if($result->RecordCount() !=0)
        {
            $str = "";
            while ($row = $result->FetchRow())
            {
                $str .= "'".$row["job_id"]."', ";
            }
            $str = substr($str, 0, -2);

            $del_sql = ("
                    DELETE FROM student_job_folder_join
                    WHERE job_id IN ($str)
                    AND folder_id ='".$folder_id."'
                    AND student_number = '".$student_number."'
                    ");
            $result = $GLOBALS['dbh']->Execute($del_sql);
            notify("Closed Jobs have been removed from ".getFolderName($folder_id,$student_number)." folder");
        }
        else
        {
            notify(getFolderName($folder_id,$student_number)." folder had no Closed Jobs");
        }
            
    }
    else
    {
        error("You must select a folder to remove closed jobs");
    }
    $show = true;
}
elseif($continue == "Rename")
{
    if($folder_id)
    {
        echo("<table border='0' cellpadding='2' cellspacing='0'>");
        echo("<tr class='row1'>");
        echo("<td>");
        echo("<table border='0' cellpadding='2' cellspacing='0' width='100%' class='row1'>");

        echo("<tr align='center'>");
        echo("<td>");
        
        $old_name = getFolderName($folder_id,$student_number);
        echo("<form action='$PHP_SELF' method='post'>");
        echo("<input type='hidden' name='select' value='view_job' />");
        echo("<input type='hidden' name='student_number' value='$student_number' />");
        echo("<input type='hidden' name='old_name' value='$old_name' />");
        echo("<input type='hidden' name='folder_id' value='$folder_id' />");
        echo("<input type='hidden' name='level1' value='folders' />");
        echo("Rename <b class='black'>".$old_name."</b> to &nbsp;");
        echo("<input type='text' name='folder_name' />");
        echo("&nbsp;<input type='submit' name='continue' value='Change' />");
        echo("</form>");

        echo("</td>");
        echo("</tr>");
        echo("</table>");

        echo("</td>");
        echo("</tr>");
        echo("</table>");
        $show = false;

    }
    else
    {
        error("You must select a folder to rename.");
        $show = true;
    }
}

elseif($continue == "Change")
{
    $pattern = array ("/<H1[^>]*>/i", "/<\/h1>/i", "/<H2[^>]*>/i", "/<\/h2>/i", "/<HTML[^>]*>/i", "/<script[^>]*>/i",
            "/<\/SCRIPT[^>]*>/i", "/<HEAD.*>/i", "/<BODY[^>]*>/i", "/<\/html>/i", "/<\/head>/i", "/<\/body>/i",
            "/<TITLE[^>]*>.*<\/title>/i", "/<STYLE[^>]*>.*<\/style>/i", "/<PRE[^>]*>/i", "/<\/PRE[^>]*>/i",
            "/<\?/i", "/\?>/i", "/<FORM[^>]*>/i", "/<\/FORM[^>]*>/i", "/<INPUT[^>]*>/i", "/<\%/i", "/\%>/i" );
    
    $replace = array ("", "", "", "");

    $folder_name = preg_replace($pattern, $replace, $folder_name);
    $folder_name = strip_tags($folder_name,'');

    if($folder_name)
    {
        $sql=("

                DELETE FROM student_job_folder_name
                WHERE student_number = '".$student_number."'
                AND folder_id = '".$folder_id."'");
        $result = $GLOBALS['dbh']->Execute($sql);

        $sql = ("
                INSERT INTO student_job_folder_name 
                (folder_name, folder_id, student_number)
                values('". addslashes(removeSlashes($folder_name))."', '".$folder_id."', '".$student_number."')
                ");
        $result = $GLOBALS['dbh']->Execute($sql);
        notify(removeSlashes($old_name)." folder has been renamed to ".getFolderName($folder_id,$student_number));
    }
    else
    {
        error("You must enter a name for renaming the folder.");

    }
    $show = true;
}

if($show)
{
 
?>
 <form action='<?php echo $PHP_SELF?>' method='post'>
<table cellspacing="0" cellpadding="10" border="0">
    <tr>
        <td colspan="1" width="100%"  align='center'>
                   <input type='hidden' name='select' value='view_job' />
            <input type='hidden' name='level1' value='folders' />
            <table border=1 cellpadding=0 cellspacing=0><tr><td>
            <table border=0 cellpadding=2>
            <tr>
                <td class='rowgrey' align=center>&nbsp;</td>
                <td class='rowgrey' align=center><b class='white'>&nbsp;Folder Name&nbsp;</b></td>
                <td class='rowgrey' align=center><b class='white'>&nbsp;Open Jobs&nbsp;</b></td>
                <td class='rowgrey' align=center><b class='white'>&nbsp;Closed Jobs&nbsp;</b></td>
            </tr>
<?php
$sql = ("
        SELECT folder_id
        FROM student_job_folder
        ");
$result = $GLOBALS['dbh']->Execute($sql);
$rowclass=1;
while ($row = $result->FetchRow())
{
    if($rowclass % 2 == 0)
    {
        $rclass = "row0d";
    }
    else
    {
        $rclass = "row1d";
    }
    $href=$PHP_SELF."&amp;select=view_job&amp;folder_id=";
    echo("<tr class='".$rclass."'>");
    echo("<td align='center'>&nbsp;&nbsp;<input type='radio' name='folder_id' value='".$row["folder_id"]."' />&nbsp;</td>"); 
    echo("<td align='center'>&nbsp;<a href='".$href.$row["folder_id"]."'>". getFolderName($row["folder_id"], $student_number)."</a>&nbsp;</td>\n");
    echo("<td align='center'>&nbsp;". getFolderSize($row["folder_id"], $student_number,"open")."&nbsp;</td>\n");
    echo("<td align='center'>&nbsp;". getFolderSize($row["folder_id"], $student_number,"closed")."&nbsp;</td>\n");
    echo("</tr>");
    $rowclass++;
}
echo("</table>");
echo("</td></tr></table>");
echo("      </td>
        </tr>
        <tr>
            <td align='center' class='row1'>");
echo("<input type='hidden' name='student_number' value='$student_number' />");
echo("<input type='submit' name='continue' value='Empty Folder' />");
echo("&nbsp;&nbsp;<input type='submit' name='continue' value='Remove Closed Jobs' />");
//echo("<tr><td>");
echo("&nbsp;&nbsp;<input type='submit' name='continue' value='Rename' />");
//echo("</form>");
//echo("<form action='$PHP_SELF' method='post'>");
echo("<input type='hidden' name='select' value='view_job' />");
echo("<input type='submit' name='level1' value='Return to Job Descriptions' />");
//echo("</td></tr>");
echo("</td></tr></table>");
echo("</form>");
}
?>

