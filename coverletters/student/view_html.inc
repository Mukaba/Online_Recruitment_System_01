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
 | Description: Diplays the user the selected coverletter.                      |
 +------------------------------------------------------------------------------+

*/

/*
if ($type_id == SPECIFIC && ($job_id == "" || $job_id == "0")){
	include( "header.inc");
	include( "coverletters/coverletter_header.inc" );
	error( "Invalid Job." );
	include( "coverletters/student/view.inc" );
	include( "coverletters/coverletter_footer.inc" );
	include( "footer.inc" );
}else{*/
	$sql = "SELECT letter, DATE_FORMAT(last_modified, '%M %d, %Y %H:%i:%S') as last_modified 
		FROM coverletter 
		WHERE coverletter_id = '$coverletter_id' AND
		student_number = '" . addslashes(stripslashes($student_number)) . "'"; 

	$result = $GLOBALS['dbh']->Execute( $sql );


	$row =  $result->FetchRow();
	
	$letter = $row["letter"];
	$last_modified = $row["last_modified"];

?>
<!DOCTYPE html PUBLIC
"-//W3C//DTD XHTML 1.0 Transitional//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>
<head>

<?php  // Stylesheets !
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

<title>Cover Letter</title></head>
<body bgcolor="white">
<form method="post" action="<?php echo( $PHP_SELF )?>">
<input type="hidden" value="coverletter" name="select" />
<input type="submit" value="Mamook" name="page" />
<input type="submit" value="Edit" name="page" />
<input type="submit" value="View PDF" name="page" />
<input type="hidden" value="1" name="PDF" />
<input type="hidden" value="<?php echo( $coverletter_id)?>" name="coverletter_id" />
</form>
Last Modified: <?php echo (($last_modified == "") ? "Never" : $last_modified)?>
<hr width="100%" />
<?php
if ($warning != ""){
echo( $warning );
echo "<hr width=\"100%\" />";
}
echo( $letter );
?>
</body>
</html>
<?php // }
?>
