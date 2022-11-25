<style>
	#uni_modal>.modal-dialog>.modal-content>.modal-footer{
		display: none
	}
</style>
<div class="continer-fluid">
	<form id="join-quiz">
		<div id="msg"></div>
	<div class="form-group">
		<label for="" class="control-label">Enter Quiz Code</label>
		<input type="text" class="form-control" id="code" name="code">
	</div>
	</form>
</div>
<div class="modal-footer">
	 	<button type="button" class="btn btn-primary" id='submit' onclick="$('#uni_modal form').submit()">Join</button>
        <button type="button" class="btn btn-secondary" onclick="location.replace(document.referrer)">Cancel</button>
</div>

<script>
	$('#join-quiz').submit(function(e){
		e.preventDefault()
		start_load()
		$('#msg').html();
		$.ajax({
			url:"ajax.php?action=check_quiz_exist",
			method:'POST',
			data:$(this).serialize(),
			success:function(resp){
				if(resp == 1){
					location.replace("index2.php?page=join_quiz&quiz="+$('#code').val())
				}else{
					$('#msg').html("<div class='alert alert-danger'>Code do not exist.</div>")
					end_load()
				}
			}
		})
	})
</script>