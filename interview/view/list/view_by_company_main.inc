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
 | Filename: view_by_company_main.inc                                           |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/

// filename: view_by_company_main.inc
//if ($department_id == "" || !in_array($department_id, $departments_in_group)){
if ($department_id == ""){
	$department_id = $auth->department;
}

?>
<form action="<?php echo($PHP_SELF . "&amp;select=view") ?>" method="post">
   <input type="hidden" name="level1" value="company" />
<?php
   //Below input is used to prevent stupid things happening when the user hits enter using Netscape.
?>
   <input type='hidden' name='submit' value='View Results' />

<table class='row1' border="0" cellpadding="4" cellspacing="0"><tr><td>
   <table width="100%">
      <tr>
         <td>Company Name:</td>
         <td><input type="text" name="company_name" maxlength="75" size="15" /></td>
      </tr>
	<?php
	if (sizeof($departments_in_group) > 1){
	?>
	  <tr>
	    <td>Department:</td>
	    <td><select name="department_id">
                <?php
                echo("<option value='0' ". (($department_id=='0') ? "selected='selected'" : "").">All in group</option>");
		foreach($departments_in_group as $department){
			if ($department == $department_id)
				{ echo "<option selected='selected' value='$department'>" . getDepartmentName($department) . "</option>\n";}
			else
				{ echo "<option value='$department'>" . getDepartmentName($department) . "</option>\n";}
			}
		?>
		</select>
	    </td>
	  </tr>
	<?php
	}
        ?>
      <tr>
         <td align="left" colspan="2">Display Past Interviews:&nbsp;<input type="checkbox" name="past" value="true" /></td>
      </tr>
      <tr> <td colspan='2'><hr /></td></tr>
      <tr>
         <td align="left"><input type="submit" name="submit" value="View Results" /></td>
	 <td align="right"><input type="submit" name="submit" value="Select from List" /></td>
<?php
// if you change the value attributes of the above submit buttons make sure you update the 
//       switch statement in view_by_company.inc
?>
      </tr>
   </table>
</td></tr></table>
</form>
