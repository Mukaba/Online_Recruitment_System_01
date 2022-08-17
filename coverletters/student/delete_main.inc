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
 | Filename: delete_main.inc                                                    |
 +------------------------------------------------------------------------------+
 | Description: Displays a list of coverletters for the student to delete       |
 | from.  They cannot delete coverletters with a status of 'ACTIVE'.            |
 +------------------------------------------------------------------------------+

*/
?>

<h3>Delete Cover Letter</h3>

<?php

$sql = "SELECT r.name as name, i.job_id as job_id, r.resume_id as resume_id, c.coverletter_id as coverletter,
		i.job_code as job_code, c.status_id as status_id, 
		c.include_transcript as include_transcript,
		DATE_FORMAT(c.last_modified, '%b %d, %Y')  as last_modified, 
		t.description as type, t.type_id as type_id,  s.description as status
        FROM coverletter as c LEFT JOIN coverletter_type as t 
			ON (t.type_id = c.type_id)
		LEFT JOIN coverletter_status AS s 
			ON (s.status_id = c.status_id) 
		LEFT JOIN resume AS r
			ON (c.resume_id = r.resume_id)
		LEFT JOIN job_info as i 
			ON (i.job_id = c.job_id)
	WHERE 
		c.student_number = '" . addslashes($student_number) . "'
	ORDER BY i.job_code";

$result = $GLOBALS['dbh']->Execute( $sql );

if ($result->RecordCount() <= 0){
	echo "<p>No cover letters currently in the system.</p>";
}else{
?>
	<table border="1" cellpadding="2" cellspacing="0">
	  <tr>
	    <td class="rowgrey" align="center"><b class="white">Job Code</b></td>
	    <td class="rowgrey" align="center"><b class="white">Type</b></td>
	    <td class="rowgrey" align="center"><b class="white">Status</b></td>
	    <td class="rowgrey" align="center"><b class="white">Transcript<br />Suppressed</b></td>
	    <td class="rowgrey" align="center"><b class="white">Resume</b></td>
	    <td class="rowgrey" align="center"><b class="white">Last Modified</b></td>
	  </tr>
<?php
	$rowclass=0;
	while($row =  $result->FetchRow()){
		$rowclass % 2 ? $class = "row0d" : $class = "row1d";
		$href1= $PHP_SELF . "&amp;select=coverletter&amp;page=Delete&amp;coverletter_id=".$row["coverletter"];
		$link = 1;
		if ($row["status_id"] == ACTIVE){
			$link = 0;
		}
		?>
		<tr>
		  <td class="<?php echo $class?>"><?php echo (($row["job_code"] == "" ) ? "&nbsp;" : (($link) ? "<a href='$href1' class='blue'>" . $row["job_code"] . "</a>" : $row["job_code"]))?></td>
		  <td class="<?php echo $class?>"><?php echo (($row["type"] == "" ) ? "&nbsp;" : (($link) ? "<a href='$href1' class='blue'>" . $row["type"] . "</a>" : $row["type"]))?></td>
		  <td class="<?php echo $class?>"><?php echo (($row["status"] == "" ) ? "&nbsp;" : $row["status"])?></td>
		  <td align="center" class="<?php echo $class?>"><?php echo (($row["include_transcript"] == "1" ) ? "No" : "Yes")?></td>
		  <td class="<?php echo $class?>"><?php echo (($row["name"] == "" ) ? "&nbsp;" : $row["name"])?></td>
		  <td class="<?php echo $class?>"><?php echo (($row["last_modified"] == "" ) ? "&nbsp;" : $row["last_modified"])?></td>
		  <input type="hidden" name="job_id[<?php echo $rowclass?>]" value="<?php echo $row["job_id"]?>" />
		  <input type="hidden" name="type_id[<?php echo $rowclass?>]" value="<?php echo $row["type_id"]?>" />
		</tr>
		<?php
		$rowclass++;
	}
?>
	</table>
<?php
}
?>
