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
 | Filename: view_contact_criteria_simple.inc                                   |
 +------------------------------------------------------------------------------+
 | Description: This is the file called up when the user wants a simpler        |
 | method to search on contacts.  This set of criteria allows them only to      |
 | search on contacts first and last names and company names.                   |
 +------------------------------------------------------------------------------+

*/


?>

<form method="post" name='myform' action="<?php echo($PHP_SELF); ?>">
<input type="hidden" name="select" value="view_contact" />
<input type='hidden' name='level1' value='contact' />

<table border="0" cellpadding="8" cellspacing="0" width="97%" class="row1">
<tr>
	<td align='center' colspan='2'>
	<input type='button' value='Go to Advanced Search' onclick='javascript:document.advanced_search_form.submit();' />
	</td>
</tr>
<tr>
    <td colspan='2'>
    <?php
        if (is_array($error_array) && sizeof($error_array) > 0)
        {
            foreach ($error_array as $error_msg)
            {
                error($error_msg);
            }
        }
    ?>
    </td>
</tr>
<tr>
	<td align="center" colspan='2'><b class='black'>
	The list of contacts returned will match the criteria entered below.  <br />Leave any fields blank that you do not wish to be used in the search.</b>
	</td>
</tr>
<tr>
        <td colspan='2'><hr width='50%' size='1' /></td>
</tr>
<tr>
	<td align='right'>First names starting with:</td>
	<td align='left'>
	<select name='search_first_name'>
		<?php
        // :TODO: Rename search_first_name and search_first_name_m to something meaningful so there are no naming conflicts for the search.
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
	<input type="text" name="search_first_name_m" size="10" maxlength="10" <?php
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
        <select name='search_last_name'>
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
        <input type='text' name='search_last_name_m' size='10' maxlength='10' <?php
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
        <input type='text' name='search_company_name' id='search_company_name' size='30' <?php
        if ($search_company_name)
        {
                echo(" value=\"" . removeSlashes(trim($search_company_name)) . "\"");
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
                <input type='button' name='company_list' value='Choose from a list' onclick='popUpChooser("search_company_name");' />
                </td>   
        </tr>           
<?php                   
}                       
?>                      
<tr>
        <td colspan='2' align='center'><hr />
        <input type='hidden' name='continue' value='simple_contact_form_done' />
        <input type='submit' value='Continue' />
        &nbsp;&nbsp;&nbsp;
        <input type='reset' value='Reset Form' />
        </td>
</tr>
</table>
</form>
<form method="post" name='advanced_search_form' action="<?php echo($PHP_SELF); ?>">
<input type="hidden" name="select" value="view_contact" />
<input type='hidden' name='level1' value='contact' />
<input type='hidden' name='switch_search' value='Go to Advanced Search' />
</form>
