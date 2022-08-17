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
| Filename: view.inc                                                           |
+------------------------------------------------------------------------------+
| Description: Ensure the select student is valid, and then displays the       |
| transcript in either pdf, or html for the user.                              |
+------------------------------------------------------------------------------+

*/

if ($department_id == '0' || $department_id == '') {
    $department_in_str = "'" . implode("', '", $departments_in_group) . "'";
    $department_in_str = "(" . $department_in_str . ")";
} else {
    $department_in_str = "('$department_id')";
}

if ($student_num != "") {
    // Get record_id for use in quickmenu
    $sql_get_recordid = "Select distinct record_id from student_department
        WHERE student_number='".$student_num."' and department_id='".$auth->department."'";
    $result_get_recordid=$GLOBALS['dbh']->Execute($sql_get_recordid);
    if ($result_get_recordid->RecordCount()==1)
    {
        $row_get_recordid = $result_get_recordid->FetchRow();
        $record_id = $row_get_recordid['record_id'];
    } elseif ($result_get_recordid->RecordCount()==0) {
        $sql_get_recordid2 = "Select distinct record_id from student_department
            WHERE student_number='".$student_num."' and department_id IN ".$department_in_str."";
        $result_get_recordid2 = $GLOBALS['dbh']->Execute($sql_get_recordid2);
        if ($result_get_recordid2->RecordCount()==1)
        {
            $row_get_recordid2 = $result_get_recordid2->FetchRow();
            $record_id = $row_get_recordid2['record_id'];
        }
    }
}

if ($student_num2 != "") {
    $student_num = $student_num2;
    // Get record_id for use in quickmenu
    $sql_get_recordid = "Select distinct record_id from student_department
        WHERE student_number='".$student_num."' and department_id='".$auth->department."'";
    $result_get_recordid=$GLOBALS['dbh']->Execute($sql_get_recordid);
    if ($result_get_recordid->RecordCount()==1)
    {
        $row_get_recordid = $result_get_recordid->FetchRow();
        $record_id = $row_get_recordid['record_id'];
    } elseif ($result_get_recordid->RecordCount()==0) {
        $sql_get_recordid2 = "Select distinct record_id from student_department
            WHERE student_number='".$student_num."' and department_id IN ".$department_in_str."";
        $result_get_recordid2 = $GLOBALS['dbh']->Execute($sql_get_recordid2);
        if ($result_get_recordid2->RecordCount()==1)
        {
            $row_get_recordid2 = $result_get_recordid2->FetchRow();
            $record_id = $row_get_recordid2['record_id'];
        }
    }

}

if ($student_name != "")
{
    $student_name = trim($student_name);
    if (preg_match("/,.*,/", $student_name))
    {
        error("You may only enter one comma in the student name field.");
    }
    elseif ($student_name == ",")
    {
        error("You must include a partial last name to view coverletters based on the name field.");
    }
    else
    {
        // Strip wild card character from student name.
        $student_name = str_replace("%", "", $student_name);
        $student_names = explode(",", $student_name);
        $first_name = trim($student_names[1]);
        $last_name = trim($student_names[0]);

        // Get record_id for use in quickmenu
        $sql_get_students = ("
            SELECT DISTINCT s.student_number 
            FROM student AS s
            INNER JOIN student_department AS sd
            ON sd.student_number=s.student_number
            INNER JOIN transcript AS t
            ON t.student_number=s.student_number
            WHERE s.first_name LIKE '".$first_name."%' 
            AND s.last_name LIKE '".$last_name."%'
            AND sd.department_id IN ".$department_in_str."
            ");
        $result_get_students=$GLOBALS['dbh']->Execute($sql_get_students);


//DEBUG STAR
/*
$sql = "
        SELECT s.student_number, CONCAT(s.first_name, ' ', s.last_name) as student_name, d.department_name,
        DATE_FORMAT(t.last_modified, '%b %d, %Y') as last_modified 
        FROM student as s 
        LEFT JOIN student_department as sd on sd.student_number=s.student_number
        INNER JOIN transcript as t on t.student_number=s.student_number
        LEFT JOIN department as d on d.department_id=sd.department_id
        WHERE
        s.first_name like '".$first_name."%' AND
        s.last_name like '".$last_name."%' AND
        sd.department_id IN ".$department_in_str."
        ORDER BY s.last_name, s.first_name";
        */
//DEBUG END

        if ($result_get_students->RecordCount()==1)
        {
            $row_get_students = $result_get_students->FetchRow();
            $student_num = $row_get_students['student_number'];
            $sql_get_recordid = "Select distinct record_id from student_department
                WHERE student_number='".$student_num."' and department_id='".$auth->department."'";
            $result_get_recordid=$GLOBALS['dbh']->Execute($sql_get_recordid);
            if ($result_get_recordid->RecordCount()==1)
            {
                $row_get_recordid = $result_get_recordid->FetchRow();
                $record_id = $row_get_recordid['record_id'];
            } elseif ($result_get_recordid->RecordCount()==0) {
                $sql_get_recordid2 = "Select distinct record_id from student_department
                    WHERE student_number='".$student_num."' and department_id IN ".$department_in_str."";
                $result_get_recordid2 = $GLOBALS['dbh']->Execute($sql_get_recordid2);
                if ($result_get_recordid2->RecordCount()==1)
                {
                    $row_get_recordid2 = $result_get_recordid2->FetchRow();
                    $record_id = $row_get_recordid2['record_id'];
                }
            }
        } else {
            $new_page='choose_student';
            unset($PDF);
            $student_num='';
            $record_id='';
        }
    }
} else {
    if (!$first_name)
    {
        $first_name = "";
    }
    if (!last_name)
    {
        $last_name = "";
    }
    if (!$student_num && !$student_num2)
    {
            $new_page='choose_student';
            unset($PDF);
            $record_id='';
    }
}

//list($student_num, $error) = find_student($student_name_select, $student_number_select, $student_num);

if($student_num)
{
if($student_num == -1){
    if ($error == "duplicate"){
        include( "misc/names.inc" );
        list($first, $last) = split(" ", $student_name_select);
        select_name($first, $last, $PHP_SELF . "&amp;select=transcript&amp;department_id=" . urlencode($department_id) . "&amp;page=" . urlencode($page) . "&amp;PDF=1");
    }else{
        unset($PDF);
    }
}else{

    $sql = "SELECT transcript, DATE_FORMAT(last_modified, '%M %d, %Y (%l:%i %p)') as last_modified 
        FROM transcript 
        WHERE student_number = '" . addslashes(stripslashes($student_num)) . "'";

    $result = $GLOBALS['dbh']->Execute( $sql );
    if ($result->RecordCount() <= 0){
        $error = ( "No transcript for " . getStudentName($student_num) . "." );
        unset( $PDF );
    }else{
        if ($page == "View PDF"){
            include ("PDF/PDFconversion/ConvertToPDF.class");
            $transcript_class = new ConvertToPDF();
            $transcript_class->convert_transcript($student_num);
        }else{
            if ($student_name_select == ""){
                $student_name_select = getStudentName($student_num);
            }

            $row =  $result->FetchRow();
            $transcript = $row["transcript"];

            ?>

                <!DOCTYPE html PUBLIC
                "-//W3C//DTD XHTML 1.0 Transitional//EN"
                "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

                <html>
                    <head><title>Transcript</title></head>
                    <body bgcolor="white">
                    <table width=100%>
                    <tr>
                    <td align='left'>
                    <form method="post" action="<?php echo $PHP_SELF?>">
                    <input type="hidden" name="select" value="transcript" />
                    <input type="hidden" name="department_id" value="<?php echo $department_id?>" />
                    <input type="hidden" name="student_num" value="<?php echo $student_num?>" />
                    <input type="hidden" name="first_name" value="<?php echo $first_name?>" />
                    <input type="hidden" name="last_name" value="<?php echo $last_name?>" />
                    <input type="hidden" name="new_page" value="<?php echo $new_page?>" />
                    <input type="hidden" name="PDF" value="1" />
                    <input type="submit" name="page" value="Mamook" />&nbsp;&nbsp;&nbsp;
                    <input type="submit" name="page" value="View PDF" />&nbsp;&nbsp;&nbsp;
                    </form>
                    </td>

                   <?php
                    // Display the quickmenu
                    echo("<td align='right'>");
                    if ($record_id && ($auth->userlevel == OFFICE || $auth->userlevel == TRAFFICDIRECTOR))
                    {
                        include('student_info/student_quickmenu.inc');
                    }
                    ?>
                    </tr>
                    </table>
                    <b>Student Name:</b> <?php echo $student_name_select?><br />
                    <b>Student Number:</b> <?php echo $student_num?><br /><br />
                    <b>Last Modified:</b> <?php echo $row["last_modified"]?>
                    <hr width="100%" />
                    <?php
                    if ($warning != ""){
                        echo $warning;
                        echo "<hr width=\"100%\" />";
                    }?>
                <?php echo $transcript?>
                    </body>
                    </html>
                    <?php
            }
        }
    }
}
?>

