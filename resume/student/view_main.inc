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
 | Filename: view_main.inc                                                      |
 +------------------------------------------------------------------------------+
 | Description: Displays all the students resumes to them allowing them to      |
 | select one to edit or view                                                   |
 +------------------------------------------------------------------------------+

*/
?>

<h3>Resumes - View</h3>
<?php
$PHP_SELF = str_replace ("&amp;page=view", "", $PHP_SELF);
$sql = "SELECT r.name, DATE_FORMAT(r.last_modified, '%b %d, %Y %H:%i') as last_modified, r.resume_id, c.description
	FROM resume as r LEFT JOIN coverletter_type as c on r.type_id = c.type_id
	WHERE r.student_number = '" . addslashes($student_number) . "'
	ORDER BY r.type_id DESC, r.name";
$result = $GLOBALS['dbh']->Execute($sql);

if ($result->RecordCount() == 0){
	echo "<p>You currently have no resumes in the system.  To add a resume please select the Add option under the resume menu.</p>";
}else{
	?>
	<form method="post" action="<?php echo $PHP_SELF?>">
	<input type="hidden" name="select" value="resume" />
	<input type="hidden" name="PDF" value="1" />
	  
<table cellspacing='0' cellpadding='0' border='1'>
<tr>
        <td>

	<table border='0' cellpadding='2'>
	  <tr>
	    <td class="rowgrey"><b class="white">&nbsp;</b></td>
	    <td class="rowgrey" align='center'><b class="white"> Resume </b></td>
	    <td class="rowgrey" align='center'><b class="white"> Type </b></td>
	    <td class="rowgrey" align='center'><b class="white"> Last Modified </b></td>
	    <td class="rowgrey" align='center'><b class="white"> Jobs </b></td>
	  </tr>

	  <?php
	  $i = 0;
	  while ($row = $result->FetchRow()){
		  (($i % 2 == 0) ? $class="row1d" : $class="row0d");
		  $i++;
		  $sql = "SELECT job_code
		  	  FROM job_info as ji, applications as a
			  WHERE a.job_id=ji.job_id AND a.student_number='$student_number' AND a.resume_id='".$row["resume_id"]."'
			  ORDER BY ji.job_code";
			  
		  $result_jobs = $GLOBALS['dbh']->Execute( $sql );

		  $jobs = "";
		  while ($row_jobs = $result_jobs->FetchRow()){
			$jobs .= $row_jobs["job_code"] . "<br />";
		  }
          
		  $jobs = substr($jobs, 0, strlen($jobs) - 6);
		  $href = $PHP_SELF . "&amp;select=resume&amp;page=View+HTML&amp;PDF=1&amp;resume_id=" . $row["resume_id"];
		  ?>
		  <tr>
		    <td class="<?php echo $class?>" valign='top'>&nbsp;<input name="resume_id" type="radio" class="<?php echo $class?>" value="<?php echo $row["resume_id"]?>" />&nbsp;</td>
		    <td class="<?php echo $class?>" valign='top'>&nbsp;<?php echo (($row["name"] == "" ) ? "" : "<a class='blue' href='$href'>" . $row["name"] . "</a>" )?>&nbsp;</td>
		    <td class="<?php echo $class?>" valign='top'>&nbsp;<?php echo (($row["description"] == "" ) ? "" : $row["description"])?>&nbsp;</td>
		    <td class="<?php echo $class?>" valign='top'>&nbsp;<?php echo (($row["last_modified"] == "" ) ? "" : $row["last_modified"])?>&nbsp;</td>
		    <td class="<?php echo $class?>" valign='top'><?php echo (($jobs == "" ) ? "(none)" : $jobs)?>&nbsp;</td>
		  </tr>
	  <?php
	  }
	  ?>
	  </table>
	</td>
   </tr>
</table>
   
	  <table border="0" width="100%">
 <?php
//$sql = "select resume_html_editable from department where department_id=\"".$auth->department."\"";
//$response = $GLOBALS['dbh']->Execute ($sql);
//$row = $response->FetchRow();

//if ($row["resume_html_editable"] == 1)
//{
//}
?> 
	    <tr>
	      <td colspan='4'>
<!--	      <td align="center"><input type="submit" name="page" value="Edit"></TD -->
	      <input type="submit" name="page" value="View HTML" /> &nbsp; &nbsp;
	      <input type="submit" name="page" value="View PDF" />
	      </td>
	    </tr>
	  </table>
	  </form>

	  <?php
}
?>
