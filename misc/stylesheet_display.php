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
 | Filename: stylesheet_display.php                                             |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/


	/* stylesheet_display.php 	- displays all classes for the stylesheet
					- used for reference
	*/

?>
<html>
<head>
<title>
<?php echo(BRAND_NAME); ?> stylesheet reference.
</title>
<link rel="stylesheet" href="css/color<?php echo $colorscheme ?>.css" type="text/css">

</head>
<body bgcolor='WHITE' marginwidth=0 marginheight=0 hspace=0 vspace=0 leftmargin=0 topmargin=0>
<br /><center><h3><?php echo $colorscheme ?></h3>
<?php if (!$colorscheme) { echo "<br /><br />"; } ?>

<table cellpadding=4 cellspacing=1 border=0 width='100%'>
	<tr><td align='center' class='row0'>row0</td></tr>
	<tr><td align='center' class='row0'>row0</td></tr>
	<tr><td align='center' class='row1'>row1</td></tr>
	<tr><td align='center' class='row1'>row1</td></tr>
	<tr><td align='center' class='row2'>row2</td></tr>
	<tr><td align='center' class='row2'>row2</td></tr>
	<tr><td align='center' class='row3'>row3</td></tr>
	<tr><td align='center' class='row3'>row3</td></tr>
	<tr><td align='center' class='row4'>row4</td></tr>
	<tr><td align='center' class='row4'>row4</td></tr>
	<tr><td align='center' class='row5'>row5</td></tr>
	<tr><td align='center' class='row5'>row5</td></tr>
	<tr><td align='center' class='rowgrey'>ROWGREY</td></tr>
	<tr><td align='center' class='row0d'>row0d</td></tr>
	<tr><td align='center' class='row0d'>row0D</td></tr>
	<tr><td align='center' class='row1d'>row1d</td></tr>
	<tr><td align='center' class='row1d'>row1D</td></tr>
	<tr><td align='center' class='row2d'>row2d</td></tr>
	<tr><td align='center' class='row2d'>row2D</td></tr>
	<tr><td align='center' class='barbase'>BARBASE</td></tr>
	<tr><td align='center' class='light'>light</td></tr>
	<tr><td align='center' class='med'>MED</td></tr>
	<tr><td align='center' class='dark'>DARK</td></tr>
	<tr><td align='center' class='cal-titlebar'>cal-titlebar</td></tr>
	<tr><td align='center' class='cal-date-area'>cal-date-area</td></tr>
	<tr><td align='center' class='cal-day-names-area'>cal-day-names-area</td></tr>
	<tr><td align='center' class='cal-today'>cal-today</td></tr>
	
</table>
<table width=100% cellpadding=4 cellspacing=1 class='notify' border=0><tr><td align='center'>TABLE.notify</td></tr></table>
<table width=100% cellpadding=4 cellspacing=1 class='error' border=0><tr><td align='center'>TABLE.error</td></tr></table>
<font face='arial'>
<b>default Bold</b><br />
<b class='green'>B.green</b><br />
<b class='white'>B.white</b><br />
<b class='black'>B.black</b><br />
<b class='red'>B.red</b><br />
<b class='blue'>B.blue</b><br />
<b class='lightblue'>B.lightblue</b><br />
<p class="row1"><b class="instruct">B.instruct</b></p><br />


</body>
</html>
