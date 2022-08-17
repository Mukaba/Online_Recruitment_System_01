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
 | Filename: locationForm.inc                                                   |
 +------------------------------------------------------------------------------+
 | Description: This form allows the user to choose the Street Address, City,   |
 | Province, Country and Directions for this interview.  This is not an         |
 | independant form, and is included by one of the add_job_form includes.       |
 +------------------------------------------------------------------------------+

*/

?>

            <tr nowrap='nowrap'>
	            <td align="right" nowrap='nowrap'>Street Address 1:&nbsp;</td>
    	        <td><input type="text" name="address1" value="<?php echo($interview->address1); ?>" size="40" maxlength="60" /></td>
            </tr>
            <tr nowrap='nowrap'>
	            <td align="right" nowrap='nowrap'>Street Address 2:&nbsp;</td>
    	        <td><input type="text" name="address2" value="<?php echo($interview->address2); ?>" size="40" maxlength="60" /></td>
            </tr>

            <tr nowrap='nowrap'>
            	<td align="right" nowrap='nowrap'>City:&nbsp;</td>
            	<td><input type="text" name="city" value="<?php echo($interview->city); ?>" size="20" maxlength="60" /></td>
            </tr>
            <tr nowrap='nowrap'>
            	<td align="right" nowrap='nowrap'>Province:&nbsp;</td>
            	<td><input type="text" name="province" value="<?php echo($interview->province); ?>" size="20" maxlength="60" /></td>
            </tr>
            <tr nowrap='nowrap'>
            	<td align="right" nowrap='nowrap'>Country:&nbsp;</td>
            	<td><input type="text" name="country" value="<?php echo($interview->country); ?>" size="20" maxlength="60" /></td>
            </tr>
            <tr nowrap='nowrap'>
            	<td align="right" nowrap='nowrap'>Directions:&nbsp;</td>
      			<td><textarea name="direction" rows="6" cols="36"><?php echo( $interview->direction ); ?></textarea></td>
            </tr>
