<!-- Sign in modal for job seeker -->
<div class="modal fade" id="delete" tabindex="-1" role="dialog" aria-labelledby="signinLabel">

    <div id="login-modal" class="modal-dialog" role="document">

        <div class="modal-content con">

            <div class="modal-header">

                <button type="button" class="close" data-dismiss="modal" aria-label="Close" name="button"><span style="color:white" aria-hidden="true">&times;</span></button>

                <img class="img img-responsive center"src="" alt="Smashjob" />

                <h4 class="modal-title text-center" id="signinLabel"><b>ACTION NEEDED</b></h4>

            </div>

            <div class="modal-body">

                <div class="container-fluid">

                    <div class="row">

                        <div class="col-md-offset-2 col-md-8">

                            <h5 class="text-center">Are you sure you want to delete this detail?</h5>

                            <div class="col-md-12">
                                <div class="col-md-6">
                                    <h6><a id="delete-action" href="remove.php?state=education&remove=<?=$jobseeker_education['id'];?>"><button class="delete-action pull-right" type="submit" name="delete-action-yes">Yes</button></a></h6>
                                </div>
                                <div class="col-md-6">
                                    <h6><a id="delete-action" href=""><button class="delete-action" type="submit" name="delete-action-no">No</button></a></h6>
                                </div>
                            </div>

                        </div>

                    </div>

                </div>

            </div>



        </div>

    </div>

</div>
