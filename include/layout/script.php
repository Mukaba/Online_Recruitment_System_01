<script src="js/jquery.js"></script>
<script src="js/bootstrap.min.js"></script>
<script src="js/owl.carousel.js"></script>
<!-- <script src="js/animatescroll.noeasing.js"></script> -->
<script src="js/animatescroll.min.js"></script>
<script src="js/jquery.datetimepicker.full.min.js"></script>
<script src="js/jquery.validate.min.js"></script>
<script src="js/additional-methods.min.js"></script>
<script src="js/jquery-ui.min.js"></script>
<script src="js/blueimp-gallery.min.js"></script>
<script src="js/script.js"></script>
<script src="js/jobseeker.js"></script>
<script src="js/employer.js"></script>


<script type="text/javascript">

  $(document).ready(function() {
      $("#owl-example").owlCarousel();

      $("#owl-demo").owlCarousel({
          autoPlay: 3000, //Set AutoPlay to 3 seconds
          items : 5,
          itemsDesktop : [1199,3],
          itemsDesktopSmall : [979,3],
          itemsMobile :	[479,1],
      });
      $('.panel-clr').click( function() {
          $(this).toggleClass('on');
      });
      // $('#section-2').animatescroll({
      //     scrollSpeed:3000,easing:'easeOutElastic'
      // });
      // $('#edu-legend-a').click(function() {
      //     $('#edit-institution-form').animatescroll({scrollSpeed:2000,easing:'easeOutBounce'});
      // });
  });
</script>
<script>
    document.getElementById('links').onclick = function (event) {
        event = event || window.event;
        var target = event.target || event.srcElement,
            link = target.src ? target.parentNode : target,
            options = {index: link, event: event},
            links = this.getElementsByTagName('a');
        blueimp.Gallery(links, options);
    };
</script>
