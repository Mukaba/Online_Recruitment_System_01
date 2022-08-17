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
| Filename: view_html.inc                                                      |
+------------------------------------------------------------------------------+
| Description: displays the coverletter to the user in HTML                    |
+------------------------------------------------------------------------------+

*/
$errorset=0;

if ($coverletter_id)
{
    $sql = "SELECT letter, DATE_FORMAT(last_modified, '%M %d, %Y (%l:%i %p)') as last_modified 
        FROM coverletter 
        WHERE coverletter_id = $coverletter_id";
    
    $result = $GLOBALS['dbh']->Execute( $sql );
    if($result->RecordCount() <= 0){
        
        $error= ( "No cover letter exists for job code - " . $job_code . " - and student - " . $student_num . "." );
        $errorset=1;
        unset( $PDF );
    }else{
        $row =  $result->FetchRow();
    }
} else {
        $error= ("Please choose a specific cover letter.");
        $errorset=1;
        unset( $PDF );
        if ($student_num_passthru)
        {
            $student_num = $student_num_passthru;
            $page="View Cover Letters";
        }
}
if ($errorset == 0)
{
$letter = $row["letter"];
$last_modified = $row["last_modified"];
?>
<!DOCTYPE html PUBLIC
"-//W3C//DTD XHTML 1.0 Transitional//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>
<head><title>Cover Letter</title></head>
<?php  // Stylesheets !
/*
        if ($client->platform == WIN)
        {
                echo("<link rel='stylesheet' href='misc/css/font-win.css' type='text/css'>");
        }
        elseif ($client->browser == IE)
        {
                echo("<link rel='stylesheet' href='misc/css/font-nonwin-ie.css' type='text/css'>");
        }
        else
        {
                echo("<link rel='stylesheet' href='misc/css/font-nonwin.css' type='text/css'>");
        }
<link rel="stylesheet" href="<?php echo $GLOBALS['colorscheme']['stylesheet']; ?>" type="text/css">
*/
?>
<body bgcolor="white">

<?php

	$result = $GLOBALS['dbh']->Execute("Select c.student_number, c.job_id, c.type_id, ji.job_code, ct.description as type 
        FROM coverletter as c
        LEFT JOIN applications as a ON a.coverletter_id=c.coverletter_id
        LEFT JOIN job_info as ji ON ji.job_id=a.job_id
        LEFT JOIN coverletter_type as ct ON ct.type_id=c.type_id
        WHERE c.coverletter_id='$coverletter_id'");
	$row = $result->FetchRow();

?>

<form method="post" action="<?php echo( $PHP_SELF )?>">
<input type="hidden" value="<?php echo ($coverletter_id)?>" name="coverletter_id" />
<input type="hidden" value="coverletter" name="select" />
<input type="hidden" value="<?php echo($record_id)?>" name="record_id" />
<input type="hidden" value="<?php echo($term)?>" name="term" />
<input type="hidden" value="<?php echo($year)?>" name="year" />
<input type="hidden" value="<?php echo($term_year_submit)?>" name="term_year_submit" />
<input type="submit" value="Mamook" name="page" />
<input type="submit" value="View PDF" name="page" />
<input type="submit" value="Edit" name="page" />
<input type="hidden" value="<?php echo $department_id?>" name="department_id" />
<input type="hidden" value="<?php echo $last?>" name="last" />
<input type="hidden" value="<?php echo $s_view?>" name="s_view" />
<input type="hidden" value="1" name="PDF" />
<input type="hidden" value="<?php echo( $row["job_id"] )?>" name="job_id" />
<input type="hidden" value="<?php echo( $row["student_number"] )?>" name="student_num" />
<input type="hidden" value="<?php echo( $row["type_id"] )?>" name="type_id" />
</form>
<b>Student Name:</b> <?php echo (getStudentName($row['student_number']))?><br />
<b>Student Number:</b> <?php echo ($row['student_number'])?><br />
<?php
if ($row['job_code'])
{
    echo("<b>Job Code:</b> ".$row['job_code']."<br />");
} else {
    echo("<b>Coverletter Type:</b> ".$row['type']."<br />");
}
?>
<b>Last Modified:</b> <?php echo (($last_modified=="") ? "Never" : $last_modified)?>
<hr width="100%" />
<?php
if ($warning != ""){
	echo( $warning );
	echo "<hr width=\"100%\">";
}
echo( $letter );
?>
</body>
</html>
<?php
}
?>
