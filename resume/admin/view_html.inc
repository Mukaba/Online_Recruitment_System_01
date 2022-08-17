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
 | Description: Displays the resume in html format. If an error occurs then a   |
 | description of the error is placed into the $error variable and the $PDF     |
 | variable is unset.  After this resume/resume.inc will be included again and  |
 | the error will be printed to the user.                                       |
 +------------------------------------------------------------------------------+

*/
 

/*this finds the generic resume of a student whose student number is specified
 *this is done because the results of the preference search allows the administrator to view the students
 *generic resume
 */

$error=0;

if ($resume_id)
{
    $sql = "SELECT student_number
        FROM resume
        WHERE resume_id = $resume_id
        ";

    $result = $GLOBALS['dbh']->Execute( $sql );
    if($result->RecordCount() <= 0){

        $errorset=1;
        unset( $PDF );
    }else{
        $row =  $result->FetchRow();
        $student_num = $row['student_number'];
    }
}
if ($student_num != "" && $resume_id == "" && $type_id == GENERIC){
	$result = $GLOBALS['dbh']->Execute("SELECT resume_id 
				FROM resume 
				WHERE type_id = '" . addslashes($type_id) . "' 
				AND student_number = '" . addslashes($student_num) . "'");
	
	$row = $result->FetchRow();
	
	if (!$result || $result->RecordCount()<=0){
		$error = "No generic resume for this student";
	}else{
		$resume_id = $row["resume_id"];
	}
}

if ($error != "0" || !isValidResume($student_num, $resume_id) || !isValidStudentNumber($student_num, "")){
	if ($error  == ""){
		$error = "Please choose a specific resume.";
	}
	$page = "View Resumes";
	unset( $PDF );
	
}else{
  $sql = "SELECT student_number, resume, name, DATE_FORMAT(last_modified, '%M %d, %Y (%l:%i %p)') as last_modified 
  FROM resume 
  WHERE student_number = '" . addslashes($student_num) . "' AND resume_id = '" . addslashes($resume_id) . "'";

  $result = $GLOBALS['dbh']->Execute( $sql );

  $row =  $result->FetchRow();
  $name = $row["name"];
  $resume = $row["resume"];
  $last_modified = $row["last_modified"];
  $student_number = $row["student_number"];
?>
 <!DOCTYPE html PUBLIC
"-//W3C//DTD XHTML 1.0 Transitional//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

 <html>
  <head>
    <title><?php echo (getStudentName($student_number))?> Resume</title>
  </head>
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
    <form method="post" action="<?php echo ($PHP_SELF);?>">
    <input type="hidden" value="<?php echo $department_id?>" name="department_id" />
    <input type="hidden" value="<?php echo($record_id)?>" name="record_id" />
    <input type="hidden" value="<?php echo($term)?>" name="term" />
    <input type="hidden" value="<?php echo($year)?>" name="year" />
    <input type="hidden" value="<?php echo($term_year_submit)?>" name="term_year_submit" />
    <input type="submit" value="Mamook" name="page" />
    <input type="submit" value="View PDF" name="page" />
<?php
    if ($auth->userlevel == OFFICE)
    {
        echo("<input type='submit' value='Edit' name='page' />");
    }
?>
    <input type="hidden" value="1" name="PDF" />
    <input type="hidden" value="<?php echo $resume_id?>" name="resume_id" />
    <input type="hidden" value="resume" name="select" />
    <input type="hidden" value="<?php echo ($student_num);?>" name="student_num" />
    </form>
    <b>Student Name:</b> <?php echo(getStudentName($student_number))?><br />
    <b>Student Number:</b> <?php echo($student_number)?><br />
    <b>Resume Name:</b> <?php echo( $name ); ?><br />
    <b>Last Modified:</b> <?php echo( $last_modified ); ?>
    <hr width="100%" />
<?php
  
  //if a warning occured while saving the resume
  if ($warning != ""){
	  echo "$warning\n";
	  echo "<hr width=\"100%\">\n";
  }
  echo ($resume);
?>
  </body>
  </html>
<?php
}
?>
