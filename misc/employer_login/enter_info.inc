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
 | Filename: enter_info.inc                                                     |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/

?>
<form method="post" name="myform" action="<?php echo($PHP_SELF); ?>">
<input type="hidden" name="select" value="employer_login" />

<table border="0" cellpadding="4" cellspacing="0" width="76%" class="row1">
<tr>
	<td colspan='2'>&nbsp;</td>
</tr>
<tr>
	<td align="center" colspan='2'><b class='black'>
	The list of contacts returned will match the criteria entered below and will only be those contacts who have a login id.</b>
	</td>
</tr>
<tr>
        <td colspan='2'><hr width='50%' size='1' /></td>
</tr>
<tr>
	<td align='right'>First names starting with:</td>
	<td align='left'>
	<select name='first_name_letter'>
		<?php
		echo("<option value='' ");
		if (!$search_first_name || $search_first_name == '')
		{
			echo("selected='selected'");
		}
		echo(">&nbsp;</option>");
		for ($i = A; $i < Z; $i++)
		{
			echo("<option value='" . $i . "' ");
			if ($search_first_name && $search_first_name == $i)
			{
				echo("selected='selected'");
			}
			echo(">".$i."</option>");
		}
		echo("<option value='$i' ");
		if ($search_first_name && $search_first_name == $i)
		{
			echo("selected='selected'");
		}
		echo(">".$i."</option>");
		?>
	</select>
	&nbsp;or&nbsp;
	<input type="text" name="first_name" size="10" maxlength="10" <?php
	if ($search_first_name_m && $search_first_name_m != '')
	{
		echo(" value=\"" . removeSlashes(trim($search_first_name_m)) . "\"");
	}
	echo(" />");
	?>
	</td>
</tr>
<tr>
        <td align='right'>Last names starting with:</td>
        <td align='left'>
        <select name='last_name_letter'>
        <?php
                echo("<option value='' ");
                if (!$search_last_name || $search_last_name == '')
                {
                        echo("selected='selected'");
                }
                echo(">&nbsp;</option>");
                for ($i = A; $i < Z; $i++)
                {
                        echo("<option value='" . $i . "' ");
                        if ($search_last_name && $search_last_name == $i)
                        {
                                echo("selected='selected'");
                        }
                        echo(">" . $i . "</option>");
                }
                echo("<option value='" . $i . "' ");
                if ($search_last_name && $search_last_name == $i)
                {
                        echo("selected='selected'");
                }
                echo(">" . $i . "</option>");
        ?>
        </select>
        &nbsp;or&nbsp;
        <input type='text' name='last_name' size='10' maxlength='10' <?php
        if ($search_last_name_m && $search_last_name_m != '')
        {
                echo(" value=\"" . removeSlashes(trim($search_last_name_m)) . "\"");
        }
        echo(" />");
        ?>
        </td>
</tr>
<tr>
        <td colspan='2' align='center'>&nbsp;</td>
</tr>
<tr>
        <td align='right'>Works for a company starting with:</td>
        <td align='left'>
        <input type='text' id='company_name' name='company_name' size='30' <?php
        if ($search_company_name)
        {
                echo(" value=\"" . removeSlashes(trim($company_name)) . "\"");
        }
        echo(" />");
        ?>
        </td>
</tr>

<?php

$sql = ("
        SELECT DISTINCT employer_id
        FROM employer_company
        WHERE company_display
        "); 
$result = $GLOBALS['dbh']->Execute($sql); 
        
if ($result->RecordCount())
{       
?>      
                
        <tr>    
                <td colspan='2' align='center'>-Or-</td>
        </tr>   
        <tr>            
                <td colspan='2' align='center'>
                <input type='button' name='company_list' value='Choose from a list' onclick='popUpChooser("company_name");' />
                </td>   
        </tr>           
<?php                   
}                       
?>                      
<tr>
        <td colspan='2' align='center'><hr />
        <input type='hidden' name='login_step' value='done_need_info' />
	<input type='submit' value='Continue' />
        &nbsp;&nbsp;&nbsp;
        <input type='reset' value='Reset Form' />
        </td>
</tr>

</table>
</form>
