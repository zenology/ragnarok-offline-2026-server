import { useEffect, useRef, type KeyboardEvent, type ReactNode } from 'react'

import Viewer from 'viewerjs'
import 'viewerjs/dist/viewer.css'

import { imageViewer } from './image-viewer.recipe'

type ImageViewerImage = {
  src: string
  alt: string
  label: string
}

type ImageViewerProps = {
  images: readonly ImageViewerImage[]
}

function ImageViewer({ images }: ImageViewerProps): ReactNode {
  const containerRef = useRef<HTMLDivElement>(null)
  const viewerRef = useRef<Viewer | null>(null)
  const styles = imageViewer()

  useEffect(() => {
    if (!containerRef.current) return

    viewerRef.current = new Viewer(containerRef.current, {
      title: true,
      toolbar: true,
      tooltip: true,
      navbar: images.length > 1,
      url: 'data-original'
    })

    return () => {
      viewerRef.current?.destroy()
      viewerRef.current = null
    }
  }, [images.length])

  function handleKeyDown(event: KeyboardEvent<HTMLImageElement>): void {
    if (event.key !== 'Enter' && event.key !== ' ') return
    event.preventDefault()
    event.currentTarget.click()
  }

  return (
    <div className={styles.root} ref={containerRef} aria-label="Location images">
      {images.map((image) => (
        <figure className={styles.item} key={`${image.src}-${image.label}`}>
          <img
            className={styles.image}
            src={image.src}
            data-original={image.src}
            alt={image.alt}
            role="button"
            tabIndex={0}
            onKeyDown={handleKeyDown}
          />
          <figcaption className={styles.caption}>{image.label}</figcaption>
        </figure>
      ))}
    </div>
  )
}

export { ImageViewer }
export type { ImageViewerImage }
