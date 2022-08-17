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
 | Filename: preferences.inc                                                    |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/


/*
 * filename: preferences.inc
 * This is the main file that directs the appropriate preference page to the user.
 *
 */

/*
Preferences:
</td>
<td width="10">&nbsp;</td>
<td valign="middle">&nbsp;<a href="<?php echo($PHP_SELF . "&amp;select=prefer_dept&amp;level1=contact"); ?>"><b class="blue">Contact</b></a>&nbsp;</td>
<td width="10">&nbsp;</td>
*/
?>
</td><td align="left" valign="top"><img src="<?php echo $GLOBALS['colorscheme']['topbar_right']; ?>" alt='' /></td></tr></table>
<br />
<table border="0" cellspacing="0" cellpadding="2" width="99%"><tr><td>


<?php
switch ($level1)
{
   case "Add New":
        $new_id = 1;
        // Falls through
        
   case "Edit":
   case "contact":
	include("preferences/staff/contact.inc");
	break;

   default:
	?>
	<form action="<?php echo($PHP_SELF . "&amp;select=".$select) ?>" method="post">

	<table border="1" cellpadding="0" cellspacing="0"><tr><td>
	   <table>
              <tr>
                 <td colspan="2">
                   <center>
                     <a href="<?=$PHP_SELF?>&amp;select=<?=$select?>&amp;level1=Add+New">Add New</a>
                   </center>
                 </td>
	      <tr>
	         <td>Select Staff Member:</td>
	         <td><select name='id'>
	         <?php
	                $result = $GLOBALS['dbh']->Execute("SELECT c.contact_id, c.first_name, c.last_name, ci.department_id, d.department_code
						FROM contact AS c, contact_internal AS ci, department AS d
						WHERE c.type='internal' AND c.contact_id=ci.contact_id AND ci.department_id=d.department_id
						ORDER BY department_code, last_name");
	                while( $row =  $result->FetchRow() )
	                        {
	                        $name = $row['first_name']." ".$row['last_name'];
	                        $id = $row['contact_id'];
	                        $dept_code = $row['department_code'];
	                        echo("<option value='$id'>$name ($dept_code)</option>");
	                        }
	        ?>
	        </select></td>
	      </tr>
	      <tr>
	         <td colspan="2" align="center">
	         <input type="submit" name="level1" value="Edit" />
	         </td>
	      </tr>
	   </table>
	</td></tr></table>
	</form>
	<?php
	break;
}

?>
</td></tr></table>
