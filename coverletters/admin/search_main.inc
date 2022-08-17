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
 | Filename: search_main.inc                                                    |
 +------------------------------------------------------------------------------+
 | Description: Allows the user to search for a selected job or student.        |
 +------------------------------------------------------------------------------+

*/

if ($page == "Search for Job"){
?>
<h3>Search</h3>
<form method="post" action="<?php echo $PHP_SELF?>">
<input type="hidden" name="select" value="coverletter" />
<input type="hidden" name="page" value="job search" />

<table border="1">
  <tr>
    <td>
      <table border="0" width="100%" cellspacing="0">
        <?php
	if (sizeof($departments_in_group) > 1){
		?>
		<tr>
		  <td>Department:</td>
		  <td><select name="department_id">
		      <option value="0">All</option>
		      <?php
		      foreach($departments_in_group as $dep){
			      echo "<option value='" . $dep . "'" . (($dep == $department_id) ? " selected='selected' " : "") . ">" . getDepartmentName($dep) . "</option>\n";
		      }
		      ?>
		      </select>
		  </td>
		</tr>
		<tr>
		  <td>&nbsp;</td>
		  <td>&nbsp;</td>
		</tr>
	<?php
	}
	?>
	
	<tr>
	  <td>Job Code:</td>
	  <td><input type="text" name="job_code" size="15" /></td>
	</tr>
	<tr>
	  <td colspan="2">&nbsp;&nbsp;&nbsp;&nbsp;OR</td>
	</tr>
	<tr>
	  <td>Company Name:</td>
	  <td><input type="text" name="company_name" size="30" /></td>
	</tr>
	<tr>
	  <td colspan="2"><br /><input type="submit" value="Search" /></td>
	</tr>
      </table>
    </td>
  </tr>
</table>
</form>



<?php
}elseif ($page == "Search for Student"){


	include ("coverletters/admin/search.inc");





}
?>
