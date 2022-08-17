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
 | Filename: verify_delete.inc                                                  |
 +------------------------------------------------------------------------------+
 | Description: Ensures that you want to delete this placement history          |
 | record, and allows you to make the student eligible again for the selected   |
 | term or not.                                                                 |
 +------------------------------------------------------------------------------+

*/
?>

</td><td align="left"><img src="<?php echo $GLOBALS['colorscheme']['topbar_right']; ?>" / alt='' /></td></tr></table>

<?php
	$sql = ("
        SELECT t.term_name, h.year, h.company_name, CONCAT(s.first_name,\" \",s.last_name) AS name 
		FROM history AS h
        INNER JOIN student AS s 
		ON s.student_number=h.student_number 
        INNER JOIN term AS t
        ON h.term_id=t.term_id
        WHERE h.history_id='".$history_id."'
        ");	
	$results = $GLOBALS['dbh']->Execute($sql);
	if ($results->RecordCount() > 0) {
		$row=$results->FetchRow();
        $term = $row['term_name'];

?>
		<h3 align='left'>Student Information - Placement History</h3>
		<center>
<?php
		$string = "Are you sure you want to unplace ".$row['name']." from ".$row['company_name']."?";
		$string .= "<p>";
		$string .= "<table><tr><td>";
		$string .= "<form method='post' name='deleteform' action='".$PHP_SELF."'>";
		$string .= "Would you like this student to be eligible for ".$term." ".$row['year']." when record is deleted? ";
        $string .= "<input type='checkbox' name='eligible' value='checked' /> Yes.";
        $string .= "<br /><br />";
		$string .= "<input type='hidden' name='select' value='delete_record' />";
		$string .= "<input type='hidden' name='history_id' value='".$history_id."' />";
		$string .= "<input type='hidden' name='menu_select' value='".$menu_select."' />";
		$string .= "<input type='button' name='btn' value=\"Delete Record\" onclick='document.deleteform.submit()' />";
		$string .= "<input type='hidden' name='edit_record' value='Delete Record' onclick='document.deleteform.submit()' />";
		$string .= "<input type='button' name='btnCancel' value='Cancel' onclick='document.cancelform.submit()' />";
		$string .= "</form>";
		$string .= "<form method='post' name='cancelform' action='".$PHP_SELF."'>";
		$string .= "<input type='HIDDEN' name='select' value='history' />";
		$string .= "<input type='HIDDEN' name='history_id' value='".$history_id."' />";
		$string .= "<input type='hidden' name='history_list' value='".$history_list."' />";
		$string .= "<input type='hidden' name='menu_select' value='".$menu_select."' />";
		$string .= "</form>";
		$string .= "</td></td></table>";
		$stinrg .= "</p>";
		instruct($string);
?>
		</center>
<?php	
	}

	else {
		echo "<h3 align='left'>Student Information - Placement History</h3>";
		notify("An error has occurred. This student does not have a placement history record");
	}
?>	
