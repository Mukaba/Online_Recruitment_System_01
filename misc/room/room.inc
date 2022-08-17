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
 | Filename: room.inc                                                           |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/


if (ROOM_BOOKING_ENABLED) {

?>

</td><td align="left" valign="top"><img src="<?php echo $GLOBALS['colorscheme']['topbar_right']; ?>" border="0" alt="" /></td></tr></table>

<?php

} else { ?>
Room Editor </td><td width="10">&nbsp;</td>
						<td valign="middle">&nbsp;<a href="<?php echo($PHP_SELF . "&amp;select=room_booking&amp;level1=add_room&amp;level2=edit_room"); ?>"><b class="blue">Edit</b></a>&nbsp;</td>
						<td width="10">&nbsp;</td>
						<td valign="middle">&nbsp;<a href="<?php echo($PHP_SELF . "&amp;select=room_booking&amp;level1=add_room&amp;level2=new_room"); ?>"><b class="blue">New</b></a>&nbsp;</td>
						<td width="10">&nbsp;</td>
						<td valign="middle">&nbsp;<a href="<?php echo($PHP_SELF . "&amp;select=room_booking&amp;level1=add_room&amp;level2=new_building"); ?>"><b class="blue">New Building</b></a>&nbsp;</td>
					<td align="left" valign="top"><img src="<?php echo $GLOBALS['colorscheme']['topbar_right']; ?>" border="0" alt="" /></td></tr></table>

<?php }



// filename: room.inc
// This is the main file that directs the appropriate room booking page to the user.

include('misc/isValid.inc');
if (ROOM_BOOKING_ENABLED)
{	// *** ONLY IF ROOM BOOKING IS ENABLED ***
	if ($cal_date) { $date = $cal_date; }
		
	if (($level1 != "add_room") && (!isValidDate($date) || (!isValidBuilding($building_id) && ($level1 == "coarse" || $level1 == "fine")) || $cal_submit))
	{
		$level1 = "date_building";
	}
	elseif ($level1 == ""){ $level1 = "coarse"; }

	if ($room_id == "" && $level1 == "room_centric") { $level1 = "date_building"; }
}	// *** ONLY IF ROOM BOOKING IS ENABLED ***
else
{	// *** ONLY IF ROOM BOOKING IS NOT ENABLED ***
	$level1 = "add_room";
}	// *** ONLY IF ROOM BOOKING IS NOT ENABLED ***

//top_menu($level1);

?>
<table border="0" cellspacing="0" cellpadding="2"><tr><td>

<?php

if (ROOM_BOOKING_ENABLED) { echo("<h3 align='left'>Room Scheduling</h3>"); } // *** ONLY IF ROOM BOOKING IS ENABLED ***

if (ROOM_BOOKING_ENABLED)
{	// *** ONLY IF ROOM BOOKING IS ENABLED ***
	switch ($level1)
	{
   	case "coarse":
      	include("misc/room/coarse.inc");
	      break;

   	case "fine":
   		include("misc/room/fine.inc");
	      break;

   	case "room_centric":
   		include("misc/room/room_centric.inc");
	      break;

		case "add_room":
			include("misc/room/add_room.inc");
			break;

   	case "date_building":
	   default:
			include("misc/room/date.inc");
      	break;
	}
}	// *** ONLY IF ROOM BOOKING IS ENABLED ***
else
{	// *** ONLY IF ROOM BOOKING IS NOT ENABLED ***
	include("misc/room/add_room.inc");
}	// *** ONLY IF ROOM BOOKING IS NOT ENABLED ***

?>
</td></tr></table>



<?php




function top_menu($level1)
{
	global $PHP_SELF;
	?>
	<table cellspacing="0" cellpadding="0" border="0">
		<tr>
			<td style="background-color: #333366;">
				<table>
					<tr>
						<td>
							&nbsp;
	<?php
		if (ROOM_BOOKING_ENABLED)
		{	// *** ONLY IF ROOM BOOKING IS ENABLED ***
	?>						
							<a href="<?php echo($PHP_SELF . "&amp;select=room_booking&amp;level1=date_building"); ?>"><b class="white">Booking</b></a>
							<b class="white">/</b>
	<?php
		}	// *** ONLY IF ROOM BOOKING IS ENABLED ***
	?>
							<a href="<?php echo($PHP_SELF . "&amp;select=room_booking&amp;level1=add_room"); ?>"><b class="white">Room Editor</b></a>
							&nbsp;
						</td>
						<td width="5">&nbsp;</td>
					</tr>
				</table>
			</td>
	<?php
	if ($level1 == "add_room")
	{
	?>
			<td class="topbar">
				<table>
					<tr>
						<td width="5">&nbsp;</td>
						<td>&nbsp;<a href="<?php echo($PHP_SELF . "&amp;select=room_booking&amp;level1=add_room&amp;level2=edit_room"); ?>"><b class="white">Edit</b></a>&nbsp;</td>
						<td width="10">&nbsp;</td>
						<td>&nbsp;<a href="<?php echo($PHP_SELF . "&amp;select=room_booking&amp;level1=add_room&amp;level2=new_room"); ?>"><b class="white">New</b></a>&nbsp;</td>
						<td width="10">&nbsp;</td>
						<!--<td>&nbsp;<a href="<?php echo($PHP_SELF . "&amp;select=room_booking&amp;level1=add_room&amp;level2=delete_room"); ?>"><b class="white">Delete</b></a>&nbsp;</td>
						<td width="10">&nbsp;</td>-->
						<td>&nbsp;<a href="<?php echo($PHP_SELF . "&amp;select=room_booking&amp;level1=add_room&amp;level2=new_building"); ?>"><b class="white">New Building</b></a>&nbsp;</td>
						<td width="10">&nbsp;</td>
					</tr>
				</table>
			</td>
	<?php
	}
	elseif ($level1 == "fine" || $level1 == "room_centric")
	{
		global $building_id;
		global $date;
		global $room_id;
		if (!$building_id && $room_id)
		{
			$result = $GLOBALS['dbh']->Execute("SELECT building_id	
						FROM room 
						WHERE room_id='" . addslashes($room_id) . "'");
			if ($result)
			{
				$building_id = ($row = $result->FetchRow()) ? $row["building_id"] : "";
				$result->Close();
			}
		}
	?>
			<td class="topbar">
				<table>
					<tr>
						<td width="5">&nbsp;</td>
						<td>&nbsp;<a href="<?php echo($PHP_SELF . "&amp;select=room_booking&amp;level1=coarse&amp;date=" . urlencode($date) . "&amp;building_id=" . urlencode($building_id)); ?>"><b class="white">Plotted Schedule</b></a>&nbsp;</td>
						<!--<td width="10">&nbsp;</td>-->
						<td width="10">&nbsp;</td>
					</tr>
				</table>
			</td>
	<?php
	}
	?>
		</tr>
	</table>
	<?php
}

?>
