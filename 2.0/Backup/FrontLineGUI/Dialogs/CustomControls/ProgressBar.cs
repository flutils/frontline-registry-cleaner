using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.Windows.Forms;
using System.ComponentModel;

namespace FrontLineGUI
{
    public class CustomProgressBar : Panel
    {
        public enum DrawMethod
        {
            Tile,
            Stretch
        }

        public enum BarType
        {
            Progress,
            Marquee
        }

        public enum MarqueeStyle
        {
            TileWrap,
            BlockWrap,
            Wave
        }

        #region Private members

        private Image offscreenImage = null;
        private Graphics offscreen = null;

#if PocketPC // See method ProgressBar.DrawImage for reason for this #if.
        private static ImageAttributes imageAttributes = null;
#endif
        #region Foreground image
        private Image foregroundImage = null;
        private int foregroundLeadingSize = 0;
        private int foregroundTrailingSize = 0;
        private DrawMethod foregroundDrawMethod = DrawMethod.Stretch;
        #endregion

        #region  Background image
        private Image backgroundImage = null;
        private int backgroundLeadingSize = 0;
        private int backgroundTrailingSize = 0;
        private DrawMethod backgroundDrawMethod = DrawMethod.Stretch;
        private Image backgroundCacheImage = null;
        #endregion

        #region Overlay image
        private Image overlayImage = null;
        private int overlayLeadingSize = 0;
        private int overlayTrailingSize = 0;
        private DrawMethod overlayDrawMethod = DrawMethod.Stretch;
        private Image overlayCacheImage = null;
        #endregion

        private BarType barType = BarType.Progress;
        private MarqueeStyle marqueeStyle = MarqueeStyle.TileWrap;

        private int value = 0;
        private int maximum = 100;
        private int marqueeWidth = 10;
        private int minimum = 0;

        #endregion

        public CustomProgressBar()
            : base()
        {
            InitializeComponent();
        }

        private void InitializeComponent()
        {
            this.SuspendLayout();
            // 
            // ProgressBar
            // 
            this.Resize += new System.EventHandler(this.ProgressBar_Resize);
            this.ResumeLayout(false);
        }

        private static void DisposeToNull(IDisposable disposable)
        {
            if (disposable != null)
            {
                disposable.Dispose();
                disposable = null;
            }
        }

        private void ProgressBar_Resize(object sender, EventArgs e)
        {
            //
            // If the progressbar is resized we dump out current offscreen and set it to null, a new one
            // will then be created as the OnPaint method will request this if it is null.
            //
            CustomProgressBar.DisposeToNull(offscreenImage);
            CustomProgressBar.DisposeToNull(offscreen);
            
            DoLayout();

            RenderCacheImages();
        }

        protected void RenderCacheImages()
        {
            CustomProgressBar.DisposeToNull(backgroundCacheImage);
            CustomProgressBar.DisposeToNull(overlayCacheImage);

            backgroundCacheImage = new Bitmap(Width, Height);
            using (Graphics backgroundCacheGraphics = Graphics.FromImage(backgroundCacheImage))
            {
                // Render the background, here we pass the entire width of the progressbar as the distance value because we
                // always want the entire background to be drawn.
                Render(backgroundCacheGraphics, backgroundImage, backgroundDrawMethod, backgroundLeadingSize, backgroundTrailingSize, this.Width);
            }

            overlayCacheImage = new Bitmap(Width, Height);
            using (Graphics overlayCacheGraphics = Graphics.FromImage(overlayCacheImage))
            {
                // Make sure that we retain our chroma key value by starting with a fully transparent overlay cache image
#if PocketPC
                overlayCacheGraphics.FillRectangle(new SolidBrush(Color.Magenta), ClientRectangle);
#else
                overlayCacheGraphics.FillRectangle(new SolidBrush(Color.Transparent), ClientRectangle);
#endif
                // Render the overlay, this way we can get neat border on our progress bar (for example)
                Render(overlayCacheGraphics, overlayImage, overlayDrawMethod, overlayLeadingSize, overlayTrailingSize, this.Width);
            }
        }


        protected void DoLayout()
        {
            //
            // This makes sure that the "ends" of the foreground progress bar is always rendered correctly
            // 
            if (foregroundImage != null)
                this.Width = Math.Max(this.Width, foregroundLeadingSize + foregroundTrailingSize);
            if (backgroundImage != null)
                this.Width = Math.Max(this.Width, backgroundLeadingSize + backgroundTrailingSize);
            if (overlayImage != null)
                this.Width = Math.Max(this.Width, overlayLeadingSize + overlayTrailingSize);
        }

        protected static void DrawImage(Graphics graphics, Image image, Rectangle destinationRectangle, Rectangle sourceRectangle)
        {
            /*
             * The only place where some porting issues arises in when drawing images, because of this the 
             * ProgressBar code does not draw using Graphics.DrawImage directly. It instead uses this 
             * wrapper method that takes care of any porting issues using pre-processor directives.
             */

#if PocketPC
            //
            // The .NET Compact Framework can not handle transparent pngs (or any images), so to achieve
            // transparancy we need to set the image attributes when drawing the image. 
            // I've decided to hard code the "chroma key" value to Color.Magenta but that can easily
            // be set by a property instead.
            // 
            if (imageAttributes == null)
            {
                imageAttributes = new ImageAttributes();
                imageAttributes.SetColorKey(Color.Magenta, Color.Magenta);
            }

            graphics.DrawImage(image, destinationRectangle, sourceRectangle.X, sourceRectangle.Y, sourceRectangle.Width, sourceRectangle.Height, GraphicsUnit.Pixel, imageAttributes);
#else
            graphics.DrawImage(image, destinationRectangle, sourceRectangle, GraphicsUnit.Pixel);
#endif

        }

        protected void Render(Graphics graphics, Image sourceImage, DrawMethod drawMethod, int leadingSize, int trailingSize, int distance)
        {
            // If we don't have an image to render just bug out, this allows us to call Render without checking sourceImage first.
            if (sourceImage == null)
                return;

            //
            // Draw the first segment of the image as defined by leadingSize, this is always drawn at (0, 0).
            // 
            CustomProgressBar.DrawImage(
                graphics,
                sourceImage,
                new Rectangle(0, 0, leadingSize, this.Height),
                new Rectangle(0, 0, leadingSize, sourceImage.Height + 1));

            //
            // Figure out where the last segment of the image should be drawn, this is always to the right of the first segment 
            // and then at the given distance minus the width of the last segment.
            //
            int trailerLeftPosition = Math.Max(leadingSize, distance - trailingSize);

            CustomProgressBar.DrawImage(
                graphics,
                sourceImage,
                new Rectangle(trailerLeftPosition, 0, trailingSize, this.Height),
                new Rectangle(sourceImage.Width - trailingSize, 0, trailingSize, sourceImage.Height + 1));


            //
            // We only draw the middle segment if the width of the first and last are less than what we need to display.
            //
            if (distance > leadingSize + trailingSize)
            {
                RenderCenterSegment(graphics, sourceImage, drawMethod, leadingSize, trailingSize, distance, trailerLeftPosition);
            }
        }

        private void RenderCenterSegment(Graphics graphics, Image sourceImage, DrawMethod drawMethod, int leadingSize, int trailingSize, int distance, int trailerLeftPosition)
        {
            switch (drawMethod)
            {
                case DrawMethod.Stretch: // This draws the middle segment stretched to fill the area between the first and last segment.
                    CustomProgressBar.DrawImage(
                        graphics,
                        sourceImage,
                        new Rectangle(leadingSize, 0, distance - (leadingSize + trailingSize), this.Height),
                        new Rectangle(leadingSize, 0, sourceImage.Width - (leadingSize + trailingSize), sourceImage.Height + 1));
                    break;
                case DrawMethod.Tile: // This draws the middle segment un-stretched as many times as required to fill the area between the first and last segment.
                    {
                        Region clipRegion = graphics.Clip;
                        int tileLeft = leadingSize;
                        int tileWidth = sourceImage.Width - (leadingSize + trailingSize);

                        // By setting clip we don't have to change the size of either the source rectangle or the destination rectangle,
                        // the clip will make sure the overflow is cropped away.
                        graphics.Clip = new Region(new Rectangle(tileLeft, 0, trailerLeftPosition - tileLeft, this.Height));
                        while (tileLeft < trailerLeftPosition)
                        {
                            CustomProgressBar.DrawImage(
                                graphics,
                                sourceImage,
                                new Rectangle(tileLeft, 0, tileWidth, this.Height),
                                new Rectangle(leadingSize, 0, tileWidth, sourceImage.Height));

                            tileLeft += tileWidth;
                        }
                        graphics.Clip = clipRegion;
                    }
                    break;
            }
        }

        private void CreateOffscreen()
        {
            //
            // In order to avoid flicker we use an offscreen and draw background, foreground and overlay to that before drawing the
            // offscreen onto the Graphics handed to us in the OnPaint method.
            //
            offscreenImage = new Bitmap(this.Width, this.Height);
            offscreen = Graphics.FromImage(offscreenImage);
        }

        protected override void OnPaintBackground(PaintEventArgs e)
        {
            // Do nothing in here as all the painting is done in OnPaint    
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            //
            // An offscreen is a must have so we make sure one is always created if it does not exists, 
            // a resize of the progressbar sets offscreenImage to null and this will then automatically 
            // create a new one with the correct dimensions.
            //
            if (offscreenImage == null)
                CreateOffscreen();

            // Render the background first using the cached image
            CustomProgressBar.DrawImage(offscreen, backgroundCacheImage, ClientRectangle, ClientRectangle);

            switch (barType)
            {
                case BarType.Progress:
                    // We only need to render the foreground if the current value is above the minimum
                    if (value > minimum)
                    {
                        // Calculate the amount of pixels (the distance) to draw.
                        int distance = (int)(((float)this.Width) * ((float)(value - minimum)) / ((float)(maximum - minimum)));
                        Render(offscreen, foregroundImage, foregroundDrawMethod, foregroundLeadingSize, foregroundTrailingSize, distance);
                    }
                    break;
                case BarType.Marquee:
                    // There are a couple of ways to render the marquee foreground so this is delegated to a method
                    RenderMarqueeForeground();
                    break;
            }

            // Render the overlay using the cached image
            CustomProgressBar.DrawImage(offscreen, overlayCacheImage, ClientRectangle, ClientRectangle);

            // Finally, draw we our offscreen onto the Graphics in the event.
            e.Graphics.DrawImage(offscreenImage, 0, 0);
        }

        private void RenderMarqueeForeground()
        {
            //
            // This method delegates the rendering further to the three 
            // different rendering methods for the marquee foreground 
            //
            switch (marqueeStyle)
            {
                case MarqueeStyle.TileWrap:
                    RenderMarqueeTileWrap();
                    break;
                case MarqueeStyle.Wave:
                    RenderMaqueeWave();
                    break;
                case MarqueeStyle.BlockWrap:
                    RenderMarqueeBlockWrap();
                    break;
            }
        }

        private void RenderMarqueeBlockWrap()
        {
            int blockWidth = foregroundLeadingSize + marqueeWidth + foregroundTrailingSize;
            float distance = (((float)(value - minimum)) / ((float)(maximum - minimum))) * (this.Width + blockWidth);
            int position = (int)(distance - blockWidth);

            CustomProgressBar.DrawImage(
                offscreen,
                foregroundImage,
                new Rectangle(position, 0, foregroundLeadingSize, this.Height),
                new Rectangle(0, 0, foregroundLeadingSize, foregroundImage.Height));

            CustomProgressBar.DrawImage(
                offscreen,
                foregroundImage,
                new Rectangle(position + foregroundLeadingSize + marqueeWidth, 0, foregroundTrailingSize, this.Height),
                new Rectangle(foregroundImage.Width - foregroundTrailingSize, 0, foregroundTrailingSize, foregroundImage.Height));

            CustomProgressBar.DrawImage(
                offscreen,
                foregroundImage,
                new Rectangle(position + foregroundLeadingSize, 0, marqueeWidth, this.Height),
                new Rectangle(foregroundLeadingSize, 0, foregroundImage.Width - (foregroundLeadingSize + foregroundTrailingSize), foregroundImage.Height + 1));
        }

        private void RenderMaqueeWave()
        {
            int blockWidth = foregroundLeadingSize + marqueeWidth + foregroundTrailingSize;
            float t = (((float)(value - minimum)) / ((float)(maximum - minimum))) * (float)(2.0 * Math.PI);
            int travelDistance = (this.Width - blockWidth) / 2;
            int position = ((this.Width - blockWidth) / 2) + (int)(Math.Sin(t) * (double)travelDistance);

            CustomProgressBar.DrawImage(
                offscreen,
                foregroundImage,
                new Rectangle(position, 0, foregroundLeadingSize, this.Height),
                new Rectangle(0, 0, foregroundLeadingSize, foregroundImage.Height));

            CustomProgressBar.DrawImage(
                offscreen,
                foregroundImage,
                new Rectangle(position + foregroundLeadingSize + marqueeWidth, 0, foregroundTrailingSize, this.Height),
                new Rectangle(foregroundImage.Width - foregroundTrailingSize, 0, foregroundTrailingSize, foregroundImage.Height));

            CustomProgressBar.DrawImage(
                offscreen,
                foregroundImage,
                new Rectangle(position + foregroundLeadingSize, 0, marqueeWidth, this.Height),
                new Rectangle(foregroundLeadingSize, 0, foregroundImage.Width - (foregroundLeadingSize + foregroundTrailingSize), foregroundImage.Height + 1));
        }

        private void RenderMarqueeTileWrap()
        {
            Region clipRegion = offscreen.Clip;
            int tileLeft = 0;
            int blockWidth = foregroundImage.Width - (foregroundLeadingSize + foregroundTrailingSize);
            int distance = (int)(((float)this.Width + blockWidth) * ((float)(value - minimum)) / ((float)(maximum - minimum)));

            // By setting clip we don't have to change the size of either the source rectangle or the destination rectangle,
            // the clip will make sure the overflow is cropped away.
            offscreen.Clip = new Region(new Rectangle(distance - blockWidth, 0, blockWidth, this.Height));
            int step = 0;
            while (tileLeft <= this.Width)
            {
                CustomProgressBar.DrawImage(
                    offscreen,
                    foregroundImage,
                    new Rectangle(blockWidth * step, 0, blockWidth, this.Height),
                    new Rectangle(foregroundLeadingSize, 0, blockWidth, foregroundImage.Height));

                tileLeft += blockWidth;
                ++step;
            }
            offscreen.Clip = clipRegion;
        }

        public void MarqueeUpdate()
        {
            // This method is a convienience method that can be called regularly to 
            // update the bar when in marquee mode
            ++value;
            if (value > maximum)
                value = minimum;

            Invalidate();
        }

        #region Public properties

#if !PocketPC
        [Category("Progressbar")]
#endif
        public int Value
        {
            get { return value; }
            set
            {
                this.value = Math.Min(maximum, Math.Max(minimum, value));
                Invalidate();
            }
        }

#if !PocketPC
        [Category("Progressbar")]
#endif
        public int Maximum
        {
            get { return maximum; }
            set { maximum = value; }
        }

#if !PocketPC
        [Category("Progressbar")]
#endif
        public int Minimum
        {
            get { return minimum; }
            set { minimum = value; }
        }

#if !PocketPC
        [Category("Progressbar")]
#endif
        public BarType Type
        {
            get { return barType; }
            set { barType = value; }
        }

#if !PocketPC
        [Category("Progressbar")]
#endif
        public MarqueeStyle Marquee
        {
            get { return marqueeStyle; }
            set { marqueeStyle = value; }
        }

#if !PocketPC
        [Category("Progressbar")]
#endif
        public int MarqueeWidth
        {
            get { return marqueeWidth; }
            set { marqueeWidth = value; }
        }


        #region Foreground properties

#if !PocketPC
        [Category("Image Foreground")]
#endif
        public Image ForegroundPicture
        {
            get { return foregroundImage; }
            set { foregroundImage = value; }
        }

#if !PocketPC
        [Category("Image Foreground")]
#endif
        public int ForegroundLeadingSize
        {
            get { return foregroundLeadingSize; }
            set { foregroundLeadingSize = value; }
        }

#if !PocketPC
        [Category("Image Foreground")]
#endif
        public int ForegroundTrailingSize
        {
            get { return foregroundTrailingSize; }
            set { foregroundTrailingSize = value; }
        }
#if !PocketPC
        [Category("Image Foreground")]
#endif
        public DrawMethod ForegroundDrawMethod
        {
            get { return foregroundDrawMethod; }
            set { foregroundDrawMethod = value; }
        }

        #endregion

        #region Background properties

#if !PocketPC
        [Category("Image Background")]
#endif
        public Image BackgroundPicture
        {
            get { return backgroundImage; }
            set { backgroundImage = value; }
        }

#if !PocketPC
        [Category("Image Background")]
#endif
        public int BackgroundLeadingSize
        {
            get { return backgroundLeadingSize; }
            set { backgroundLeadingSize = value; }
        }

#if !PocketPC
        [Category("Image Background")]
#endif
        public int BackgroundTrailingSize
        {
            get { return backgroundTrailingSize; }
            set { backgroundTrailingSize = value; }
        }

#if !PocketPC
        [Category("Image Background")]
#endif
        public DrawMethod BackgroundDrawMethod
        {
            get { return backgroundDrawMethod; }
            set { backgroundDrawMethod = value; }
        }

        #endregion

        #region Overlay properties

#if !PocketPC
        [Category("Image Overlay")]
#endif
        public Image OverlayPicture
        {
            get { return overlayImage; }
            set { overlayImage = value; }
        }

#if !PocketPC
        [Category("Image Overlay")]
#endif
        public int OverlayLeadingSize
        {
            get { return overlayLeadingSize; }
            set { overlayLeadingSize = value; }
        }

#if !PocketPC
        [Category("Image Overlay")]
#endif
        public int OverlayTrailingSize
        {
            get { return overlayTrailingSize; }
            set { overlayTrailingSize = value; }
        }

#if !PocketPC
        [Category("Image Overlay")]
#endif
        public DrawMethod OverlayDrawMethod
        {
            get { return overlayDrawMethod; }
            set { overlayDrawMethod = value; }
        }

        #endregion

        #endregion
    }
}
