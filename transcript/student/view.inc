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
 | Description: Displays the student's transcript in html, pdf, or displays     |
 | buttons allowing them to select which they want to view.                     |
 +------------------------------------------------------------------------------+

*/

if ($page == "View HTML"){
	$sql = "SELECT transcript, DATE_FORMAT(last_modified, '%M %d, %Y %H:%i:%S') as last_modified
	        FROM transcript
	        WHERE student_number = '$student_number'";
		
		$result = $GLOBALS['dbh']->Execute( $sql );
		
		$row =  $result->FetchRow();
		$transcript = $row["transcript"];
		
		?>
<!DOCTYPE html PUBLIC
"-//W3C//DTD XHTML 1.0 Transitional//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

		<html>
		<head><title>Transcript</title></head>
		<body bgcolor="white">
		<form method="post" action="<?php echo $PHP_SELF?>">
		<input type="hidden" name="select" value="transcript" />
		<input type="hidden" name="PDF" value="1" />
		<input type="submit" name="page" value="View PDF" />&nbsp;&nbsp;&nbsp;
		<input type="submit" name="page" value="Mamook" />
		</form>
		Last Modified: <?php echo $row["last_modified"]?>
		<hr width="100%" />
		<?php echo $transcript?>
		</body>
		</html>
		<?php
}elseif ($page == "View PDF"){
	include( "PDF/PDFconversion/ConvertToPDF.class" );
	$transcript = new ConvertToPDF();
	$transcript->convert_transcript( $student_number );	
}else{
	?>
	<table border="0" width="100%" cellpadding="5">
	  <tr>
	    <td>
	      <h3>View Transcript</h3>
	<?php
	$sql = "SELECT count(*) as t_count FROM transcript WHERE student_number = '" . addslashes($student_number) . "'";
	$result = $GLOBALS['dbh']->Execute($sql);
	$row = $result->FetchRow();
	if ($row["t_count"] <= 0){
		?>
		<p>You do not have a transcript in the system.</p>
		<?php
	}else{
		?>
		<form method="post" action="<?php echo $PHP_SELF?>">
	        <input type="hidden" name="select" value="transcript" />
		<input type="hidden" name="PDF" value="1" />
      		<input type="submit" name="page" value="View PDF" />
		<input type="submit" name="page" value="View HTML" />
  	    </form>
  	    <?php
	}
	?>
	    </td>
	  </tr>
	</table>
<?php
}
?>
