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
 | Filename: search_application.inc                                             |
 +------------------------------------------------------------------------------+
 | Description: Files searches the applicantions for a job of a specified       |
 | criteria.  The HTML tags are striped from the results when they are          |
 | displayed on the screen.                                                     |
 +------------------------------------------------------------------------------+

*/

$search_items = explode(",", $search_item);

switch ($search_select[0]){
	case "resume":
		$sql = "SELECT r.resume as letter, r.student_number as student_number
			FROM resume as r, coverletter as c
			WHERE r.student_number = c.student_number AND r.resume_id = c.resume_id AND c.job_id = $job_id 
				AND (r.resume REGEXP '[[:<:]]" . addslashes(stripslashes(trim($search_items[0]))) . "[[:>:]]' OR ";
		for($k=1;$k<count($search_items);$k++){
			$sql .= "r.resume REGEXP '[[:<:]]" . addslashes(stripslashes(trim($search_items[$k]))) . "[[:>:]]' OR ";
		}
		$sql = substr($sql, 0, strlen($sql)-3) . ") ORDER BY r.student_number";
		echo $sql;
		break;
	case "transcript":
		$sql = "SELECT r.transcript as letter, r.student_number as student_number
			FROM transcript as r, coverletter as c
			WHERE r.student_number = c.student_number AND c.job_id = $job_id 
				AND (r.transcript REGEXP '[[:<:]]" . addslashes(stripslashes(trim($search_items[0]))) . "[[:>:]]' OR ";
		for($k=1;$k<count($search_items);$k++){
			$sql .= "r.trasnscript REGEXP '[[:<:]]" . addslashes(stripslashes(trim($search_items[$k]))) . "[[:>:]]' OR ";
		}
		$sql = substr($sql, 0, strlen($sql)-3) . ") ORDER BY r.student_number";
		break;	
	case "coverletter":
		$sql = "SELECT letter, student_number
			FROM coverletter
			WHERE job_id = $job_id AND status_id IS NULL
				AND (letter REGEXP '[[:<:]]" . addslashes(stripslashes(trim($search_items[0]))) . "[[:>:]]' OR ";
		for($k=1;$k<count($search_items);$k++){
			$sql .= "letter REGEXP '[[:<:]]" . addslashes(stripslashes(trim($search_items[$k]))) . "[[:>:]]' OR ";
		}
		$sql = substr($sql, 0, strlen($sql)-3) . ") ORDER BY student_number";

		break;
	
	default:
		$error = 1;
		error("Please select what you wish to search (transcript, cover letter, resume)");
		include ("coverletters/admin/search_application_main.inc");
		$sql = "";		
}
if (!$error){
$result = $GLOBALS['dbh']->Execute ( $sql );
if ($result->RecordCount() <= 0){
	echo "<h3>Search Results</h3>No Matches";
}else{
	?>
	<h3>Search Results</h3>
	<table border="1">
	  <tr>
	    <td class="rowgrey"><b class="white">student number</b></td>
	    <td class="rowgrey"><b class="white">Matches</b></td>
	    <td class="rowgrey"><b class="white">Line</b></td>
	  </tr>
	<?php
	$i = 0;
	while ($row =  $result->FetchRow()){
		($i % 2 == 0) ? $class="row0d" : $class="row1d";
		$i ++;
		$letter = $row["letter"];
		$line_array = explode("\n", $letter);
		$lines = 0;
		$match = array();
		foreach ($line_array as $line){
			$lines ++;
			$line = strip_tags($line);
			foreach($search_items as $item){
				if ($item != "" && preg_match("/\b" . trim($item) . "\b/i", $line)){
					$match[$lines] = $line;
				}
			}
		}
		
		$size = sizeof($match);
		if ($size > 0){
			?>
			<tr>
			  <td rowspan="<?php echo $size?>" class="<?php echo $class?>"><?php echo $row["student_number"]?></td>
			<?php
			$first = 1;
			foreach ($match as $lines=>$line){
				if ($first == 1){
			  	      ?> 
			  		<td class="<?php echo $class?>"><?php echo htmlspecialchars($line)?></td>
					<td class="<?php echo $class?>"><?php echo $lines?></td>
				      </tr>
				      <?php
					$first = 0;
				}else{
				      ?>
				      <tr>
			  		<td class="<?php echo $class?>"><?php echo htmlspecialchars($line)?></td>
					<td class="<?php echo $class?>"><?php echo $lines?></td>
				      </tr>
				      <?php
				}
			}
		}
		
	}
	?>
	</table>
	<?php
}
}



?>
