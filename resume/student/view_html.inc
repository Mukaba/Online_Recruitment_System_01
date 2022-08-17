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
 | Description: Displays the selected resume to the student in either PDF or    |
 | HTML format.                                                                 |
 +------------------------------------------------------------------------------+

*/

	//view html
	?>
    <!DOCTYPE html PUBLIC
"-//W3C//DTD XHTML 1.0 Transitional//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

	  <html>
	  <head><title>Resume</title>

<?php 
        if ($client->platform == WIN)
        {
                echo("<link rel='stylesheet' href='misc/css/font-win.css' type='text/css' />");
        }
        elseif ($client->browser == IE)
        {
                echo("<link rel='stylesheet' href='misc/css/font-nonwin-ie.css' type='text/css' />");
        }
        else
        {
                echo("<link rel='stylesheet' href='misc/css/font-nonwin.css' type='text/css' />");
        }
?>

	<link rel="stylesheet" href="<?php echo $GLOBALS['colorscheme']['stylesheet']; ?>" type="text/css" />



  	  </head>
	  <body bgcolor="white">
	<?php
	  $sql = "SELECT resume, name, DATE_FORMAT(last_modified, '%M %d, %Y %H:%i:%S') as last_modified 
	  	  FROM resume 
		  WHERE student_number = '" . addslashes($student_number) . "' AND resume_id = '" . addslashes($resume_id) . "'";
  
 	  $result = $GLOBALS['dbh']->Execute( $sql );

	  $row =  $result->FetchRow();
	  $name = $row["name"];
	  $resume = $row["resume"];
	  $last_modified = $row["last_modified"];

	?>

	  <form method="post" action="<?php echo( $PHP_SELF ) ?>">
	  <input type="hidden" value="1" name="PDF" />
	  <input type="hidden" value="<?php echo $resume_id?>" name="resume_id" />
	  <input type="submit" name="page" value="View PDF" />
	  <input type="hidden" value="resume" name="select" />
	  <input type="submit" name="page" value="Edit" />
	  <input type="submit" name="page" value="Mamook" /> 
	 </form>
	  Resume: <?php echo $name?> <br />
	  Last modified: <?php echo( $last_modified ); ?>
	  <hr width="100%" />
	<?php
	  
	//after saving the resume is displayed in html.  If the user has html tags that 
	//they shouldn't the warning message is set in the save.inc file and printed here
	  if ($warning != ""){
		  echo "$warning\n";
		  echo "<hr width=\"100%\" />";
	  }
	  echo $resume;
	?>

 	 </body>
 	 </html>
