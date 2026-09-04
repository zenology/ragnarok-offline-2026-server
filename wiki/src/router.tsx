import type { ReactNode } from 'react'

import { createRootRoute, createRoute, createRouter, Link, Outlet } from '@tanstack/react-router'

import { SiteShell } from '@/components/templates'
import BlackMarketPage from '@/pages/black-market'
import CostumePage from '@/pages/costumes'
import JobChangesPage from '@/pages/job-changes'
import LandingPage from '@/pages/landing'

declare module '@tanstack/react-router' {
  interface Register {
    router: typeof router
  }
}

function RootLayout(): ReactNode {
  return (
    <SiteShell>
      <Outlet />
    </SiteShell>
  )
}

function NotFoundPage(): ReactNode {
  return (
    <main aria-labelledby="not-found-title" style={{ padding: '96px 24px', textAlign: 'center' }}>
      <h1 id="not-found-title">Page not found</h1>
      <p>The guide page you requested does not exist.</p>
      <Link to="/">Return to the landing page</Link>
    </main>
  )
}

const rootRoute = createRootRoute({
  component: RootLayout,
  notFoundComponent: NotFoundPage
})

const landingRoute = createRoute({
  getParentRoute: () => rootRoute,
  path: '/',
  component: LandingPage
})

const blackMarketRoute = createRoute({
  getParentRoute: () => rootRoute,
  path: '/black-market',
  component: BlackMarketPage
})

const costumesRoute = createRoute({
  getParentRoute: () => rootRoute,
  path: '/costumes',
  component: CostumePage
})

const jobChangesRoute = createRoute({
  getParentRoute: () => rootRoute,
  path: '/job-changes',
  component: JobChangesPage
})

const routeTree = rootRoute.addChildren([
  landingRoute,
  blackMarketRoute,
  costumesRoute,
  jobChangesRoute
])

export const router = createRouter({ routeTree })
